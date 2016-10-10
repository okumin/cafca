#include "offset_index.h"

#include <cmath>
#include <boost/filesystem.hpp>
#include <fstream>

namespace cafca {
namespace commit_log {

using boost::iostreams::mapped_file;
using boost::optional;
using boost::none;

OffsetIndex::OffsetIndex(const std::string &filename,
                         const uint64_t base_offset)
    : filename_(filename), base_offset_(base_offset) {
  const auto exists = boost::filesystem::exists(filename);
  position_ = exists ? max_index_bytes : 0;
  if (!exists) {
    std::fstream file;
    file.open(filename, std::ios::out);
    file.close();
  }
  auto offset = 0;
  mmap_.open(filename, mapped_file::readwrite, max_index_bytes, offset);
  size_ = position_ / 8;
  last_offset_ = GetLastPosition().position;
}

const OffsetPosition OffsetIndex::GetLastPosition() const {
  if (size_ == 0) {
    return OffsetPosition{base_offset_, 0};
  } else {
    const auto offset = base_offset_ + RelativeOffset(size_ - 1);
    const auto position = PhysicalPosition(size_ - 1);
    return OffsetPosition{offset, position};
  }
}

const OffsetPosition OffsetIndex::Lookup(const uint64_t target_offset) {
  const auto slot = IndexSlotFor(target_offset);
  if (slot) {
    const auto offset = base_offset_ + RelativeOffset(slot.get());
    const auto position = PhysicalPosition(slot.get());
    return OffsetPosition{offset, position};
  } else {
    return OffsetPosition{base_offset_, 0};
  }
}

void OffsetIndex::Append(const OffsetPosition &position) {
  assert(!is_full());
  if (size_ != 0 && position.offset <= GetLastPosition().offset) {
    throw std::runtime_error("This index is in an invalid state.");
  }

  const auto relative_position = static_cast<uint32_t>(position.offset - base_offset_);
  const auto begin = mmap_.data() + (relative_position * 8 / sizeof(char));
  memcpy(begin, &relative_position, sizeof(relative_position));
  memcpy(begin + 4 / sizeof(char), &position.position, sizeof(position.position));
  last_offset_ = position.offset;
}

void OffsetIndex::Trim() {
  const auto new_size = size_ * 8;
  const auto offset = 0;
  mmap_.close();
  mmap_.open(filename_, mapped_file::readwrite, round(new_size, 8), offset);
  max_entries_ = static_cast<uint32_t>(mmap_.max_length / 8);
}

const uint32_t OffsetIndex::RelativeOffset(const uint32_t n) const {
  const auto begin = mmap_.const_data() + (n * 8 / sizeof(char));
  uint32_t offset;
  memcpy(&offset, begin, sizeof(offset));
  return offset;
}

const uint32_t OffsetIndex::PhysicalPosition(const uint32_t n) const {
  const auto begin = mmap_.const_data() + (n * 8 / sizeof(char) + 4 / sizeof(char));
  uint32_t position;
  memcpy(&position, begin, sizeof(position));
  return position;
}

const boost::optional<uint32_t> OffsetIndex::IndexSlotFor(const uint64_t target_offset) const {
  if (size_ == 0) return none;

  const uint64_t relative_offset = target_offset - base_offset_;
  if (RelativeOffset(0) > relative_offset) return none;

  uint32_t low = 0;
  uint32_t high = size_ - 1;

  while (low < high) {
    const uint32_t mid = static_cast<uint32_t>(ceil(high / 2.0 + low / 2.0));
    const uint32_t found = RelativeOffset(mid);
    if (found == relative_offset) {
      return mid;
    } else if (found < relative_offset) {
      low = mid;
    } else {
      high = mid - 1;
    }
  }

  return low;
}

}
}
