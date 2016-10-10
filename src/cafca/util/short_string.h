#ifndef CAFCA_SHORT_STRING_H
#define CAFCA_SHORT_STRING_H

#include <stdint.h>
#include <string>
#include "binary.h"
#include "optional.h"
namespace cafca {
namespace util {

struct ShortString {
  std::string value;
};

uint16_t inline short_string_length(const std::string &value) {
  return static_cast<uint16_t>(2 + value.size());
}

template <> inline const boost::optional<DecodeResult<ShortString>> Decode(
    const uint8_t *buffer,
    const uint32_t remaining
) noexcept {
  return bind(
      Decode<uint16_t>(buffer, remaining),
      [buffer, remaining](const DecodeResult<uint16_t>& size) -> const boost::optional<DecodeResult<ShortString>> {
        if (remaining < size.read_size + size.value) return boost::none;
        const auto bytes = std::unique_ptr<uint8_t[]>(new uint8_t[size.value]);
        memcpy(bytes.get(), buffer + 2, size.value);
        const auto str = std::string(bytes.get(), bytes.get() + size.value);
        const auto short_str = ShortString{str};
        return boost::make_optional(DecodeResult<ShortString>{
            short_str,
            size.read_size + size.value
        });
      }
  );
}

template <> inline const boost::optional<uint32_t> Encode(
    uint8_t *buffer,
    const uint32_t remaining,
    const ShortString& value) noexcept {
  const uint16_t size = short_string_length(value.value) - 2;
  Encode(buffer, remaining, size);
  memcpy(buffer + sizeof(size), value.value.data(), size);
  return static_cast<uint32_t>(sizeof(size) + size);
}

}
}

#endif //CAFCA_SHORT_STRING_H
