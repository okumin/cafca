#ifndef CAFCA_COMMIT_LOG_FILE_H
#define CAFCA_COMMIT_LOG_FILE_H

#include <string>
#include <cafca/util/file_channel.h>
#include <cafca/message/message.h>
#include <cafca/message/message_set.h>
#include <cafca/message/file_message_set.h>
#include "offset_index.h"
namespace cafca {
namespace commit_log {

class CommitLogFile {
 public:
  explicit CommitLogFile(const std::string &filename);

  CommitLogFile(const CommitLogFile&) = delete;

  CommitLogFile& operator=(const CommitLogFile&) = delete;

  CommitLogFile(CommitLogFile&&) = delete;

  CommitLogFile& operator=(CommitLogFile&&) = delete;

  void Append(const cafca::message::MessageSet &messages);

  void Trim();

  /**
   * Search forward for the file position of the last offset
   * that is greater than or equal to the target offset;
   */
  const boost::optional<cafca::commit_log::OffsetPosition> GetPositionGETarget(
      const uint64_t target_offset,
      const uint32_t starting_position
  );

  const uint32_t size_in_bytes() const {
    return size_in_bytes_;
  }

  std::unique_ptr<cafca::message::FileMessageSet> Read(const uint32_t position, const uint32_t size);

  const uint64_t GetLastModifiedTime();

 private:
  static constexpr uint32_t message_meta_length =
      cafca::message::Message::offset_length +
          cafca::message::Message::message_size_length;
  static constexpr uint32_t message_overhead =
      cafca::message::Message::key_offset +
          cafca::message::Message::value_size_length -
          cafca::message::Message::offset_length;

  const std::string filename_;
  std::shared_ptr<cafca::util::FileChannel> channel_;
  uint32_t size_in_bytes_;
};

}
}

#endif //CAFCA_COMMIT_LOG_FILE_H
