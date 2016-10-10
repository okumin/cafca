#ifndef CAFCA_BINARY_H
#define CAFCA_BINARY_H

#include <boost/optional/optional.hpp>
#include <boost/endian/conversion.hpp>
namespace cafca {
namespace util {

template <typename T>
struct DecodeResult {
  const T value;
  const uint32_t read_size;
};

template <typename T> const boost::optional<DecodeResult<T>> DecodePrimitive(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  if (sizeof(T) > remaining) return boost::none;
  T value = 0;
  memcpy(&value, buffer, sizeof(T));
  value = boost::endian::big_to_native(value);
  return DecodeResult<T>{value, static_cast<uint32_t>(sizeof(T))};
}

template <typename T> const boost::optional<uint32_t> EncodePrimitive(
    uint8_t *buffer,
    const uint32_t remaining,
    const T value) noexcept {
  if (sizeof(T) > remaining) return boost::none;
  const auto big = boost::endian::native_to_big(value);
  memcpy(buffer, &big, sizeof(T));
  return static_cast<uint32_t>(sizeof(T));
}

template <typename T> const boost::optional<DecodeResult<T>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept;

template <typename T> const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const T& value) noexcept;

template <> inline const boost::optional<DecodeResult<uint8_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<uint8_t>(buffer, remaining);
}

template <> inline const boost::optional<DecodeResult<uint16_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<uint16_t>(buffer, remaining);
}

template <> inline const boost::optional<DecodeResult<uint32_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<uint32_t>(buffer, remaining);
}

template <> inline const boost::optional<DecodeResult<int32_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<int32_t>(buffer, remaining);
}

template <> inline const boost::optional<DecodeResult<uint64_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<uint64_t>(buffer, remaining);
}

template <> inline const boost::optional<DecodeResult<int64_t>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return cafca::util::DecodePrimitive<int64_t>(buffer, remaining);
}

template <> inline const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const uint16_t& value) noexcept {
  return cafca::util::EncodePrimitive<uint16_t>(buffer, remaining, value);
}

template <> inline const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const uint32_t& value) noexcept {
  return cafca::util::EncodePrimitive<uint32_t>(buffer, remaining, value);
}

template <> inline const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const uint64_t& value) noexcept {
  return cafca::util::EncodePrimitive<uint64_t>(buffer, remaining, value);
}

template <> inline const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const int64_t& value) noexcept {
  return cafca::util::EncodePrimitive<int64_t>(buffer, remaining, value);
}

}
}

#endif //CAFCA_BINARY_H
