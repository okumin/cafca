#ifndef CAFCA_PRODUCER_RESPONSE_H
#define CAFCA_PRODUCER_RESPONSE_H

#include <stdint.h>
#include <vector>
#include <cafca/server/cluster/topic_and_partition.h>
#include <cafca/util/byte_buffer.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

struct ProducerResponseStatus {
  const uint16_t error;
  const uint64_t offset;
};

class ProducerResponse {
 public:
  using Statuses = std::vector<std::tuple<cafca::server::cluster::TopicAndPartition, ProducerResponseStatus>>;

  ProducerResponse(const ProducerResponse&) = delete;
  ProducerResponse& operator=(const ProducerResponse&) = delete;
  ProducerResponse(ProducerResponse&&) = default;
  ProducerResponse& operator=(ProducerResponse&&) = default;

  ProducerResponse(const uint32_t correlation_id, const Statuses &statuses);

  cafca::util::ByteBuffer to_bytes() const;

 private:
  const uint32_t correlation_id_;
  const Statuses statuses_;
};

}
}
}
}

#endif //CAFCA_PRODUCER_RESPONSE_H
