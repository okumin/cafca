#ifndef CAFCA_TOPIC_METADATA_RESPONSE_H
#define CAFCA_TOPIC_METADATA_RESPONSE_H

#include <cafca/server/cluster/broker_endpoint.h>
#include <cafca/server/cluster/topic_metadata.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

struct TopicMetadataResponse {
  const std::vector<cafca::server::cluster::BrokerEndpoint> brokers;
  const std::vector<cafca::server::cluster::TopicMetadata> topics_metadata;
  const uint32_t correlation_id;

  cafca::util::ByteBuffer to_bytes() const;
};

}
}
}
}

namespace cafca {
namespace util {

template<> inline const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const cafca::server::api::command::TopicMetadataResponse &value) noexcept {

  buffer.Put(value.correlation_id);
  buffer.Put(static_cast<uint32_t>(value.brokers.size()));
  for (const auto &broker : value.brokers) {
    buffer.Put(broker);
  }

  buffer.Put(static_cast<uint32_t>(value.topics_metadata.size()));
  for (const auto &metadata : value.topics_metadata) {
    buffer.Put(metadata);
  }
  return buffer.position() - index;
}

}
}

#endif //CAFCA_TOPIC_METADATA_RESPONSE_H
