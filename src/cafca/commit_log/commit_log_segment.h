#ifndef CAFCA_LOG_SEGMENT_H
#define CAFCA_LOG_SEGMENT_H

#include "commit_log_file.h"
#include "commit_log_offset_metadata.h"
namespace cafca {
namespace commit_log {

struct FetchInfo {
  const CommitLogOffsetMetadata metadata;
  std::shared_ptr<cafca::message::FileMessageSet> messages;
};

class CommitLogSegment {
 public:
  explicit CommitLogSegment(const std::string &log_filename,
                            const std::string &index_filename,
                            const uint64_t start_offset);

  CommitLogSegment(const CommitLogSegment&) = delete;

  CommitLogSegment& operator=(const CommitLogSegment&) = delete;

  CommitLogSegment(CommitLogSegment&&) = delete;

  CommitLogSegment& operator=(CommitLogSegment&&) = delete;

  void Append(const uint64_t offset, const cafca::message::MessageSet &messages);

  void Trim();

  const uint64_t GetLastModifiedTime();

  const boost::optional<FetchInfo> Read(const uint64_t startOffset,
                                        const uint32_t maxSize,
                                        const uint64_t maxPosition);

  const uint32_t log_size() const {
    return log_->size_in_bytes();
  }

  const bool index_is_full() const {
    return index_->is_full();
  }

  const uint64_t base_offset() const {
    return base_offset_;
  }

 private:
  static constexpr uint32_t index_internal_bytes = 4096;

  const uint64_t base_offset_;
  std::unique_ptr<cafca::commit_log::CommitLogFile> log_;
  std::unique_ptr<cafca::commit_log::OffsetIndex> index_;

  uint32_t bytes_since_last_index_ = 0;

};

}
}

#endif //CAFCA_LOG_SEGMENT_H
