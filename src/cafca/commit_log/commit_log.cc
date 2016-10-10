#include "commit_log.h"

#include <boost/filesystem.hpp>
namespace cafca {
namespace commit_log {

using cafca::commit_log::CommitLogSegment;
namespace fs = boost::filesystem;

CommitLog::CommitLog(const std::string &topic,
                     const uint32_t partition,
                     const std::string &dir) : topic_(topic),
                                               partition_(partition),
                                               dir_(dir) {
  boost::filesystem::create_directories(dir);
}

const LogAppendInfo CommitLog::Append(cafca::message::MessagesBuffer &messages) {
  const auto first_offset = next_offset_metadata_.message_offset;
  const auto result = messages.AssignOffsets(next_offset_metadata_.message_offset);
  const auto last_offset = std::get<0>(result) - 1;
  const auto message_set = std::get<1>(result);

  const auto segment = RollIfFull(message_set.size_in_bytes());
  segment->Append(first_offset, message_set);
  next_offset_metadata_ = CommitLogOffsetMetadata{
      last_offset + 1,
      segment->base_offset(),
      segment->log_size()
  };

  return LogAppendInfo{first_offset, last_offset};
}

const std::vector<uint64_t> CommitLog::FetchOffsetsBefore(const int64_t time, const uint32_t maxNumOffsets) {
  const auto size = active_segment().get()->log_size() > 0 ? segments_.size() + 1 : segments_.size();
  auto offsets = std::vector<std::pair<uint64_t, uint64_t>>(size);
  for (auto kv : segments_) {
    offsets.push_back(std::make_pair(kv.second->base_offset(), kv.second->GetLastModifiedTime()));
  }
  if (active_segment().get()->log_size() > 0) {
    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    offsets.push_back(std::make_pair(next_offset_metadata_.message_offset, now.count()));
  }

  uint64_t startIndex = 0;
  if (time == -1) {
    startIndex = offsets.size() - 1;
  } else if (time == -2) {
    startIndex = 0;
  } else {
    startIndex = offsets.size() - 1;
    while (true) {
      if (offsets[startIndex].second <= time) break;
      if (startIndex == 0) break;
      startIndex -= 1;
    }
  }

  const auto resultSize = maxNumOffsets < (startIndex + 1) ? maxNumOffsets : (startIndex + 1);
  auto result = std::vector<uint64_t>(resultSize);
  for (size_t i = 0; i < resultSize; ++i) {
    result.push_back(offsets[i].first);
    startIndex -= 1;
  }
  std::sort(result.begin(), result.end(), std::greater<uint64_t>());
  return result;
}

const FetchInfo CommitLog::Read(const uint64_t startOffset, const uint32_t maxLength) {
  const auto nextMetadata = next_offset_metadata_;
  const auto next = nextMetadata.message_offset;
  if (startOffset == next) {
    return FetchInfo {
        nextMetadata,
        std::make_shared<cafca::message::NullMessageSet>()
    };
  }

  boost::optional<std::pair<uint64_t, std::shared_ptr<cafca::commit_log::CommitLogSegment>>> entry = boost::none;
  for (auto kv : segments_) {
    if (kv.first <= startOffset) entry = kv;
  }

  if (startOffset > next || !entry) {
    throw std::runtime_error("Out of range. start_offset: " + std::to_string(startOffset) + ", next: " + std::to_string(next));
  }

  while (true) {
    uint64_t maxPosition = 0;
    if (entry.get().first == next_offset_metadata_.segment_base_offset) {
      maxPosition = static_cast<uint64_t>(next_offset_metadata_.relative_position_in_segment);
    } else {
      maxPosition = entry->second->log_size();
    }
    const auto info = std::move(entry->second->Read(startOffset, maxLength, maxPosition));
    if (info) return info.get();

    auto higher = segments_.upper_bound(entry->first);
    if (higher == segments_.end()) {
      break;
    }
    entry = *higher;
  }

  return FetchInfo {
      nextMetadata,
      std::make_shared<cafca::message::NullMessageSet>()
  };
}

std::shared_ptr<CommitLogSegment> CommitLog::Roll() {
  const auto new_offset = next_offset_metadata_.message_offset;
  const auto new_log = log_filename(new_offset);
  const auto new_index = index_filename(new_offset);

  // TODO: Checks filename duplication
  const auto entry = active_segment();
  if (entry) entry.get()->Trim();

  const auto new_segment = std::make_shared<CommitLogSegment>(
      new_log, new_index, new_offset
  );
  segments_.insert(LogSegmentMap::value_type(
      new_offset,
      new_segment
  ));
  next_offset_metadata_ = CommitLogOffsetMetadata{
      new_offset,
      new_offset,
      new_segment->log_size()
  };

  return new_segment;
}

std::shared_ptr<CommitLogSegment> CommitLog::RollIfFull(const uint32_t message_size) {
  const auto segment = active_segment();
  if (
      !segment ||
      segment.get()->log_size() > log_segment_bytes - message_size ||
      segment.get()->index_is_full()
      ) {
    return Roll();
  } else {
    return segment.get();
  }
}

}
}
