#include <cafca/util/logging.h>
#include <cafca/util/short_string.h>
#include "fetch_request.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

FetchRequest::FetchRequest(cafca::util::ByteBuffer &buffer) {
  version_id_ = buffer.Get<uint16_t>().get();
  cafca::util::debug("version_id: ", version_id_);
  correlation_id_ = buffer.Get<uint32_t>().get();
  cafca::util::debug("correlation_id: ", correlation_id_);
  client_id_ = buffer.Get<cafca::util::ShortString>().get().value;
  cafca::util::debug("client_id: ", client_id_);
  replica_id_ = buffer.Get<uint32_t>().get();
  cafca::util::debug("replica_id: ", replica_id_);
  max_wait_ = buffer.Get<uint32_t>().get();
  min_bytes_ = buffer.Get<uint32_t>().get();
  max_bytes_ = version_id_ < 3 ? 2147483647 : buffer.Get<uint32_t>().get();
  const auto topicCount = buffer.Get<uint32_t>().get();
  cafca::util::debug("topic_count: ", topicCount);
  info_ = std::unordered_map<cafca::server::cluster::TopicAndPartition, cafca::server::api::command::PartitionFetchInfo>();
  for (uint32_t i = 0; i < topicCount; ++i) {
    const auto topic = buffer.Get<cafca::util::ShortString>().get().value;
    cafca::util::debug("topic: ", topic);
    const auto partitionCount = buffer.Get<uint32_t>().get();
    for (uint32_t j = 0; j < partitionCount; ++j) {
      const auto partitionId = buffer.Get<uint32_t>().get();
      cafca::util::debug("partition_id: ", partitionId);
      const auto offset = buffer.Get<uint64_t>().get();
      cafca::util::debug("offset: ", offset);
      const auto fetchSize = buffer.Get<uint32_t>().get();
      cafca::util::debug("fetch_size: ", fetchSize);
      info_.insert(std::make_pair(
          cafca::server::cluster::TopicAndPartition { topic, partitionId },
          cafca::server::api::command::PartitionFetchInfo { offset, fetchSize }
      ));
    }
  }
}

}
}
}
}
