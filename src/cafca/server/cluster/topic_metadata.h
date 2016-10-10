#ifndef CAFCA_TOPIC_METADATA_H
#define CAFCA_TOPIC_METADATA_H

#include "broker_endpoint.h"
namespace cafca {
namespace server {
namespace cluster {

struct PartitionMetadata {
  const uint32_t partition_id;
  const boost::optional<BrokerEndpoint> leader;
  const std::vector<BrokerEndpoint> replicas;
  const std::vector<BrokerEndpoint> isr;
  const uint16_t error_code;

  const uint32_t size_in_bytes() const {
    return 2 + 4 + 4 + 4 + 4 * static_cast<uint32_t>(replicas.size()) + 4 + 4 * static_cast<uint32_t>(isr.size());
  }
};

struct TopicMetadata {
  const std::string topic;
  const std::vector<PartitionMetadata> partition_metadata;
  const uint16_t error_code;

  const uint32_t size_in_bytes() const {
    auto sum = 2 + static_cast<uint32_t>(cafca::util::short_string_length(topic)) + 4;
    for (const auto &pm : partition_metadata) {
      sum += pm.size_in_bytes();
    }
    return sum;
  }
};

}
}
}

namespace cafca {
namespace util {

template <> inline const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const cafca::server::cluster::PartitionMetadata& value) noexcept {
  uint32_t offset = index;
  const auto error = Encode(buffer, offset, value.error_code);
  if (!error) return boost::none;
  offset += error.get();
  const auto partition_id = Encode(buffer, offset, value.partition_id);
  if (!partition_id) return boost::none;
  offset += partition_id.get();
  const auto leader = Encode(
      buffer,
      offset,
      value.leader ? value.leader.get().id : static_cast<uint32_t>(-1)
  );
  if (!leader) return boost::none;
  offset += leader.get();

  const auto replicas_size = Encode(buffer, offset, static_cast<uint32_t>(value.replicas.size()));
  if (!replicas_size) return boost::none;
  offset += replicas_size.get();
  for (auto &replica : value.replicas) {
    const auto rep = Encode(buffer, offset, replica.id);
    if (!rep) return boost::none;
    offset += rep.get();
  }

  const auto isr_size = Encode(buffer, offset, static_cast<uint32_t>(value.isr.size()));
  if (!isr_size) return boost::none;
  offset += isr_size.get();
  for (auto &isr : value.isr) {
    const auto rep = Encode(buffer, offset, isr.id);
    if (!rep) return boost::none;
    offset += rep.get();
  }

  return offset - index;
}

template <> inline const boost::optional<uint32_t> Encode(
    ByteBuffer &buffer,
    const uint32_t index,
    const cafca::server::cluster::TopicMetadata& value) noexcept {
  using Result = boost::optional<uint32_t>;
  if (!buffer.position(index)) return boost::none;
  if (!buffer.Put(value.error_code)) return boost::none;
  if (!buffer.Put(cafca::util::ShortString{value.topic})) {
    return boost::none;
  }
  if (!buffer.Put<uint32_t>(static_cast<uint32_t>(value.partition_metadata.size()))) {
    return boost::none;
  }
  for (auto &meta : value.partition_metadata) {
    if (!buffer.Put(meta)) return boost::none;
  }
  return buffer.position() - index;
}

}
}


#endif //CAFCA_TOPIC_METADATA_H
