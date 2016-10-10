#ifndef CAFCA_OFFSET_INDEX_H
#define CAFCA_OFFSET_INDEX_H

#include <string>
#include <stdint.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/optional/optional.hpp>
namespace cafca {
namespace commit_log {

struct OffsetPosition {
  /**
   * The logical log offset;
   */
  const uint64_t offset;

  /**
   * The physical position in the log file.
   */
  const uint32_t position;
};

class OffsetIndex {
 public:
  explicit OffsetIndex(const std::string &filename, const uint64_t base_offset);

  OffsetIndex(const OffsetIndex&) = delete;

  OffsetIndex& operator=(const OffsetIndex&) = delete;

  OffsetIndex(OffsetIndex&&) = delete;

  OffsetIndex& operator=(OffsetIndex&&) = delete;

  /**
   * Returns the position of the last entry.
   */
  const OffsetPosition GetLastPosition() const;

  /**
   * Returns the largest offset less than or equal to the given offset.
   */
  const OffsetPosition Lookup(const uint64_t target_offset);

  /**
   * Append the position to the index file.
   */
  void Append(const OffsetPosition& position);

  void Trim();

  const bool is_full() const {
    return size_ > max_entries_;
  }

 private:
  static constexpr int max_index_bytes = 10 * 1024 * 1024;

  const uint32_t RelativeOffset(const uint32_t n) const;

  const uint32_t PhysicalPosition(const uint32_t n) const;

  const boost::optional<uint32_t> IndexSlotFor(const uint64_t target_offset) const;

  static const uint32_t round(const uint32_t number, const uint32_t factor) {
    return factor * (number / factor);
  }

  const std::string filename_;
  const uint64_t base_offset_;

  uint32_t position_;
  uint32_t size_;
  uint64_t last_offset_;
  uint32_t max_entries_ = max_index_bytes / 8;

  boost::iostreams::mapped_file mmap_;
};

}
}

#endif //CAFCA_OFFSET_INDEX_H
