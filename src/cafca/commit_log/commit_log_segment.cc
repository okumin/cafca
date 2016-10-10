#include "commit_log_segment.h"
#include "commit_log.h"

namespace cafca {
namespace commit_log {

CommitLogSegment::CommitLogSegment(const std::string &log_filename,
                                   const std::string &index_filename,
                                   const uint64_t start_offset)
    : base_offset_(start_offset),
      log_(std::unique_ptr<CommitLogFile>(new CommitLogFile(log_filename))),
      index_(std::unique_ptr<OffsetIndex>(new OffsetIndex(index_filename, start_offset))) {}

void CommitLogSegment::Append(const uint64_t offset,
                              const cafca::message::MessageSet &messages) {
  if (bytes_since_last_index_ > index_internal_bytes) {
    index_->Append(OffsetPosition{offset, log_->size_in_bytes()});
    bytes_since_last_index_ = 0;
  }

  log_->Append(messages);
  bytes_since_last_index_ += messages.size_in_bytes();
}

void CommitLogSegment::Trim() {
  index_->Trim();
  log_->Trim();
}

const uint64_t CommitLogSegment::GetLastModifiedTime() {
  return log_->GetLastModifiedTime();
}

const boost::optional<FetchInfo> CommitLogSegment::Read(const uint64_t startOffset,
                                                        const uint32_t maxSize,
                                                        const uint64_t maxPosition) {
  const auto mapping = index_->Lookup(startOffset);
  const auto startPosition = log_->GetPositionGETarget(startOffset, mapping.position);
  if (!startPosition) return boost::none;

  const auto metadata = CommitLogOffsetMetadata { startOffset, base_offset_, startPosition.get().position };
  const auto diff = static_cast<uint32_t>(maxPosition - startPosition.get().position);
  const auto length = diff < maxSize ? diff : maxSize;
  return std::move(FetchInfo {
      metadata,
      log_->Read(startPosition.get().position, length)
  });
}

}
}
