#ifndef CAFCA_BROKER_ENDPOINT_H
#define CAFCA_BROKER_ENDPOINT_H

#include <stdint.h>
#include <string>
#include <cafca/util/byte_buffer.h>
#include <cafca/util/short_string.h>
namespace cafca {
namespace server {
namespace cluster {

struct BrokerEndpoint {
  const uint32_t id;
  const std::string host;
  const uint32_t port;

  const uint32_t size_in_bytes() const {
    return 4 + 4 + cafca::util::short_string_length(host);
  }
};

}
}
}

namespace cafca {
namespace util {

template <> inline const boost::optional<ByteBufferDecodeResult<cafca::server::cluster::BrokerEndpoint>> Decode(
    const ByteBuffer &buffer,
    const uint32_t index
) noexcept {
  using Result = BBDR<cafca::server::cluster::BrokerEndpoint>;
  using MaybeResult = const boost::optional<Result>;
  return bind(
      Decode<uint32_t>(buffer, index),
      [&, index](const BBDR<uint32_t>& id) -> MaybeResult {
        return bind(
           Decode<ShortString>(buffer, index + id.read_size),
           [&, index](const BBDR<ShortString>& host) -> MaybeResult {
             return map(
                 Decode<uint32_t>(buffer, index + id.read_size + host.read_size),
                 [&, index](const BBDR<uint32_t>& port) -> Result {
                   return ByteBufferDecodeResult<cafca::server::cluster::BrokerEndpoint>{
                       cafca::server::cluster::BrokerEndpoint{
                           id.value,
                           host.value.value,
                           port.value
                       },
                       id.read_size + host.read_size + port.read_size
                   };
                 }
             );
           }
        );
      }
  );
}

template <> inline const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const cafca::server::cluster::BrokerEndpoint& value) noexcept {
  using Result = const boost::optional<uint32_t>;
  return bind(
      Encode(buffer, index, value.id),
      [&, index](const uint32_t id) -> Result {
        return bind(
            Encode(buffer, index + id, ShortString{value.host}),
            [&, index](const uint32_t host) -> Result {
              const auto result = Encode(buffer, index + id + host, value.port);
              if (result) {
                return id + host + result.get();
              } else {
                return result;
              }
            }
        );
      }
  );
}

}
}

#endif //CAFCA_BROKER_ENDPOINT_H
