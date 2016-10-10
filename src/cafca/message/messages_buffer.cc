#include "messages_buffer.h"

namespace cafca {
namespace message {

MessagesBuffer::MessagesBuffer(const cafca::util::ByteBuffer &buffer)
    : buffer_(buffer) {}

const std::tuple<uint64_t, cafca::message::MessageSet> MessagesBuffer::AssignOffsets(
    uint64_t offset_counter) {
  buffer_.Rewind();
  while (buffer_.has_remaining()) {
    buffer_.Put<uint64_t>(offset_counter);
    const auto messageSize = buffer_.Get<uint32_t>().get();
    assert(buffer_.Skip(messageSize));
    offset_counter += 1;
  }
  buffer_.Rewind();
  return std::make_tuple(offset_counter, MessageSet(buffer_));
}

}
}
