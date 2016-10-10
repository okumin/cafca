#ifndef CAFCA_COMMIT_LOG_H
#define CAFCA_COMMIT_LOG_H

#include <string>
#include <map>
#include <cafca/message/messages_buffer.h>
#include "commit_log_segment.h"
#include "commit_log_offset_metadata.h"
namespace cafca {
namespace commit_log {

struct LogAppendInfo {
  const uint64_t first_offset;
  const uint64_t last_offset;
};

class CommitLog {
 public:
  explicit CommitLog(const std::string &topic,
                     const uint32_t partition,
                     const std::string &dir);

  CommitLog(const CommitLog&) = delete;
  CommitLog& operator=(const CommitLog&) = delete;
  CommitLog(CommitLog&&) = delete;
  CommitLog& operator=(CommitLog&&) = delete;

  const LogAppendInfo Append(cafca::message::MessagesBuffer &messages);

  const std::vector<uint64_t> FetchOffsetsBefore(const int64_t time, const uint32_t maxNumOffsets);

  const FetchInfo Read(const uint64_t startOffset, const uint32_t maxLength);

  const uint64_t log_end_offset() const {
    return next_offset_metadata_.message_offset;
  }

 private:
  /**
   * base_offset -> log_segment
   */
  using LogSegmentMap = std::map<uint64_t, std::shared_ptr<CommitLogSegment>>;

  static constexpr uint32_t log_segment_bytes = 1 * 1024 * 1024 * 1024;

  std::shared_ptr<CommitLogSegment> Roll();

  std::shared_ptr<CommitLogSegment> RollIfFull(const uint32_t message_size);

  const std::string log_filename(const uint64_t offset) {
    return dir_ + std::to_string(offset) + ".log";
  }

  const std::string index_filename(const uint64_t offset) {
    return dir_ + std::to_string(offset) + ".index";
  }

  boost::optional<std::shared_ptr<CommitLogSegment>> active_segment() const {
    if (segments_.empty()) return boost::none;
    return segments_.rbegin()->second;
  }

  const std::string topic_;
  const uint32_t partition_;
  const std::string dir_;

  CommitLogOffsetMetadata next_offset_metadata_ = CommitLogOffsetMetadata{0, 0, 0};
  LogSegmentMap segments_ =
      std::map<uint64_t, std::shared_ptr<cafca::commit_log::CommitLogSegment>>();
};

}
}

#endif //CAFCA_COMMIT_LOG_H
