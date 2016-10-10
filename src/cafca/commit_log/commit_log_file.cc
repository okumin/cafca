#include <cafca/message/message.h>
#include "commit_log_file.h"

namespace cafca {
namespace commit_log {

using cafca::commit_log::OffsetPosition;
using cafca::message::Message;
using cafca::util::FileChannel;

CommitLogFile::CommitLogFile(const std::string &filename)
    : filename_(filename),
      channel_(std::make_shared<FileChannel>(filename, 0)),
      size_in_bytes_(0) {}

void CommitLogFile::Append(const cafca::message::MessageSet &messages) {
  size_in_bytes_ += channel_->Write(messages.buffer());
}

void CommitLogFile::Trim() {
  const auto target_size = size_in_bytes();
  channel_->Truncate(target_size);
  channel_->Position(target_size);
  size_in_bytes_ = target_size;
}

const boost::optional<cafca::commit_log::OffsetPosition> CommitLogFile::GetPositionGETarget(
    const uint64_t target_offset,
    const uint32_t starting_position) {
  uint32_t position = starting_position;
  const uint32_t log_size = size_in_bytes_;
  while (position + message_meta_length < log_size) {
    const auto offset_option = channel_->Read<uint64_t>(position);
    const auto message_size_option = channel_->Read<uint32_t>(
        position + Message::offset_length
    );
    if (!offset_option || !message_size_option) {
      return boost::none;
    }

    auto offset = offset_option.get();
    if (offset >= target_offset) return OffsetPosition{offset, position};

    auto message_size = message_size_option.get();
    if (message_size < message_overhead) {
      return boost::none;
    }

    position += message_meta_length + message_size_option.get();
  }

  return boost::none;
}

std::unique_ptr<cafca::message::FileMessageSet> CommitLogFile::Read(const uint32_t position, const uint32_t size) {
  const auto limit = (uint32_t) (channel_->Size() - position);
  const auto readSize = size < limit ? size : limit;
  return std::unique_ptr<cafca::message::FileMessageSet>(
      new cafca::message::FSMessageSet(channel_, position, readSize)
  );
}

const uint64_t CommitLogFile::GetLastModifiedTime() {
  return channel_->GetLastModifiedTime();
}

}
}
