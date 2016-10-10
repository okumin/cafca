#ifndef CAFCA_OFFSET_REQUEST_H
#define CAFCA_OFFSET_REQUEST_H

#include <unordered_map>
#include <cafca/server/cluster/topic_and_partition.h>
#include <cafca/util/byte_buffer.h>

namespace cafca {
namespace server {
namespace api {
namespace command {

class OffsetRequest {
 public:
  OffsetRequest(cafca::util::ByteBuffer &buffer);

  OffsetRequest(const OffsetRequest&) = delete;
  OffsetRequest& operator=(const OffsetRequest&) = delete;
  OffsetRequest(OffsetRequest&&) = default;
  OffsetRequest& operator=(OffsetRequest&&) = default;

  const std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<int64_t, uint32_t>> request_info() const {
    return request_info_;
  };

  const uint32_t correlation_id() const {
    return correlation_id_;
  }

  const std::string& client_id() const {
    return client_id_;
  }

 private:
  std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<int64_t, uint32_t>> request_info_;
  uint16_t version_id_;
  uint32_t correlation_id_;
  std::string client_id_;
  int32_t replica_id_;
};

}
}
}
}

#endif //CAFCA_OFFSET_REQUEST_H
