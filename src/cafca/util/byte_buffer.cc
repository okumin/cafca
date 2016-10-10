#include "byte_buffer.h"

namespace cafca {
namespace util {

ByteBuffer::ByteBuffer(const uint32_t capacity)
    : position_(0),
      limit_(capacity),
      start_offset_(0),
      capacity_(capacity),
      buffer_(std::make_shared<Buffer>(std::vector<uint8_t>(capacity))) {}

ByteBuffer::ByteBuffer(const ByteBuffer &other,
                       const uint32_t start_offset,
                       const uint32_t limit)
    : position_(0),
      limit_(limit),
      start_offset_(start_offset),
      capacity_(other.capacity_ - start_offset),
      buffer_(other.buffer_) {
  assert(limit <= capacity_);
}

}
}
