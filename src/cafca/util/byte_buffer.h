#ifndef CAFCA_BYTE_BUFFER_H
#define CAFCA_BYTE_BUFFER_H

#include <vector>
#include <stdint.h>
#include <boost/optional.hpp>
#include "binary.h"
#include "optional.h"
namespace cafca {
namespace util {

class ByteBuffer;

template <typename T>
struct ByteBufferDecodeResult {
  const T value;
  const uint32_t read_size;
};

template <typename T> const boost::optional<ByteBufferDecodeResult<T>> Decode(
    const ByteBuffer &buffer,
    const uint32_t index
) noexcept;

template <typename T> const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const T &value) noexcept;

class ByteBuffer {
 public:
  using Buffer = std::vector<uint8_t>;

  explicit ByteBuffer(const uint32_t capacity);

  const uint32_t position() const noexcept {
    return position_;
  }

  const uint32_t limit() const noexcept {
    return limit_;
  }

  /**
   * Returns whether unread bytes exists.
   */
  const bool has_remaining() const noexcept {
    return position_ < limit_;
  }

  /**
   * Rewinds its position.
   */
  void Rewind() noexcept {
    position_ = 0;
  }

  ByteBuffer Slice() const noexcept {
    return ByteBuffer(*this, position_, limit_ - position_);
  }

  ByteBuffer Slice(const uint32_t offset, const uint32_t size) const {
    return ByteBuffer(*this, offset, size);
  }

  const bool Skip(const uint32_t size) noexcept {
    const bool possible = (position_ + size <= limit_);
    if (possible) position_ += size;
    return possible;
  }

  const uint32_t remaining() const noexcept {
    return limit_ - position_;
  }

  const uint32_t remaining(const uint32_t index) const noexcept {
    return limit_ - index;
  }

  const bool position(const uint32_t new_position) noexcept {
    if (position_ > limit_) return false;
    position_ = new_position;
    return true;
  }

  /**
   * Reads this buffer from its position and returns T.
   */
  template <typename T> const boost::optional<T> Get() noexcept {
    const auto current = position_;
    const auto result = cafca::util::Decode<T>(*this, position_);
    if (!result) {
      position_ = current;
      return boost::none;
    } else {
      position_ = current + result->read_size;
      return result->value;
    }
  }

  /**
   * Reads this buffer from the index and returns T.
   */
  template <typename T> const boost::optional<T> Get(const uint32_t index) const noexcept {
    const auto result = cafca::util::Decode<T>(*this, index);
    if (result) {
      return result->value;
    } else {
      return boost::none;
    }
  }

  /**
   * Writes T on this buffer.
   */
  template <typename T> bool Put(const T &value) noexcept {
    const auto current = position_;
    const auto result = cafca::util::Encode<T>(*this, position_, value);
    if (result) {
      position_ = current + result.get();
      return true;
    } else {
      position_ = current;
      return false;
    }
  }

  uint8_t* pointer(const uint32_t index) const {
    return &(*buffer_.get())[start_offset_ + index];
  }

  uint8_t* pointer() const {
    return pointer(position());
  }

 private:

  ByteBuffer(const ByteBuffer &other,
             const uint32_t start_offset,
             const uint32_t limit);

  uint32_t position_;
  uint32_t limit_;
  const uint32_t start_offset_;
  const uint32_t capacity_;
  const std::shared_ptr<Buffer> buffer_;
};

template <typename T> const boost::optional<ByteBufferDecodeResult<T>> Decode(
    const ByteBuffer &buffer,
    const uint32_t index
) noexcept {
  const auto result = Decode<T>(buffer.pointer(index), buffer.remaining(index));
  if (result) {
    return ByteBufferDecodeResult<T>{result.get().value, result.get().read_size};
  } else {
    return boost::none;
  }
}

template <typename T> const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const T &value) noexcept {
  return Encode<T>(buffer.pointer(index), buffer.remaining(index), value);
}

template <typename T> using BBDR = ByteBufferDecodeResult<T>;

}
}

#endif //CAFCA_BYTE_BUFFER_H
