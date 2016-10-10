#ifndef CAFCA_OFFSET_RESPONSE_H
#define CAFCA_OFFSET_RESPONSE_H

#include <stdint.h>
#include <unordered_map>
#include <cafca/server/cluster/topic_and_partition.h>
#include <vector>
#include <cafca/util/byte_buffer.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

class OffsetResponse {
 public:
  OffsetResponse(const uint32_t correlation_id, const std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<uint16_t, std::vector<uint64_t>>> &partition_error_and_offsets);

  OffsetResponse(const OffsetResponse&) = delete;
  OffsetResponse& operator=(const OffsetResponse&) = delete;
  OffsetResponse(OffsetResponse&&) = default;
  OffsetResponse& operator=(OffsetResponse&&) = default;

  cafca::util::ByteBuffer to_bytes() const;

 private:
  const uint32_t correlation_id_;
  const std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<uint16_t, std::vector<uint64_t>>> partition_error_and_offsets_;
};

}
}
}
}

#endif //CAFCA_OFFSET_RESPONSE_H
