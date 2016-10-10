#ifndef CAFCA_FETCH_REQUEST_H
#define CAFCA_FETCH_REQUEST_H

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <cafca/server/cluster/topic_and_partition.h>
#include <cafca/util/byte_buffer.h>
#include "partition_fetch_info.h"
namespace cafca {
namespace server {
namespace api {
namespace command {

class FetchRequest {
 public:
  FetchRequest(cafca::util::ByteBuffer &buffer);

  FetchRequest(const FetchRequest&) = delete;
  FetchRequest& operator=(const FetchRequest&) = delete;
  FetchRequest(FetchRequest&&) = default;
  FetchRequest& operator=(FetchRequest&&) = default;

  const std::unordered_map<cafca::server::cluster::TopicAndPartition, cafca::server::api::command::PartitionFetchInfo>& info() const {
    return info_;
  };

  const uint32_t correlation_id() const {
    return correlation_id_;
  }

  const uint16_t version_id() const {
    return version_id_;
  }

 private:
  uint16_t version_id_;
  uint32_t correlation_id_;
  std::string client_id_;
  uint32_t replica_id_;
  uint32_t max_wait_;
  uint32_t min_bytes_;
  uint32_t max_bytes_;
  std::unordered_map<cafca::server::cluster::TopicAndPartition, cafca::server::api::command::PartitionFetchInfo> info_;
};

}
}
}
}

#endif //CAFCA_FETCH_REQUEST_H
