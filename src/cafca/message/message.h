#ifndef CAFCA_MESSAGE_H
#define CAFCA_MESSAGE_H

#include <stdint.h>
#include <cafca/util/byte_buffer.h>
#include <cafca/util/optional.h>
namespace cafca {
namespace message {

class Message {
 public:
  static constexpr uint32_t offset_offset = 0;
  static constexpr uint32_t offset_length = 8;
  static constexpr uint32_t message_size_offset = offset_offset + offset_length;
  static constexpr uint32_t message_size_length = 4;
  static constexpr uint32_t crc_offset = message_size_offset + message_size_length;
  static constexpr uint32_t crc_length = 4;
  static constexpr uint32_t magic_offset = crc_offset + crc_length;
  static constexpr uint32_t magic_length = 1;
  static constexpr uint32_t attributes_offset = magic_offset + magic_length;
  static constexpr uint32_t attributes_length = 1;
  static constexpr uint32_t key_size_offset = attributes_offset + attributes_length;
  static constexpr uint32_t key_size_length = 4;
  static constexpr uint32_t key_offset = key_size_offset + key_size_length;
  static constexpr uint32_t value_size_length = 4;
  static const uint32_t min_header_size = offset_length +
      message_size_length +
      crc_length +
      magic_length +
      attributes_length +
      key_size_length +
      value_size_length;

  explicit Message(const cafca::util::ByteBuffer &buffer);

  const uint64_t offset() const {
    return buffer_.Get<uint64_t>(offset_offset).get();
  }

  const uint32_t size() const {
    return buffer_.Get<uint32_t>(message_size_offset).get();
  }

 private:
  /**
   * 1. 8 bytes offset
   * 2. 4 bytes size(n)
   * 3. n bytes message
   */
  const cafca::util::ByteBuffer buffer_;
};

}
}

namespace cafca {
namespace util {

namespace {

constexpr uint32_t kMessageMetaDataSize =
    cafca::message::Message::offset_length + cafca::message::Message::message_size_length;

}

template <> inline const boost::optional<ByteBufferDecodeResult<cafca::message::Message>> Decode(
    const ByteBuffer &buffer,
    const uint32_t index
) noexcept {
  return bind(
      Decode<uint32_t>(buffer, index + cafca::message::Message::offset_length),
      [&, index](const BBDR<uint32_t>& size) -> const boost::optional<BBDR<cafca::message::Message>> {
        const auto read_size = kMessageMetaDataSize + size.value;
        if (index + read_size > buffer.limit()) return boost::none;
        return boost::make_optional(ByteBufferDecodeResult<cafca::message::Message>{
            cafca::message::Message(buffer.Slice(index, read_size)),
            read_size
        });
      }
  );
}

}
}

#endif //CAFCA_MESSAGE_H
