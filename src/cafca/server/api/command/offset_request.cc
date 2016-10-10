#include <cafca/util/logging.h>
#include <cafca/util/short_string.h>
#include "offset_request.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

OffsetRequest::OffsetRequest(cafca::util::ByteBuffer &buffer) {
  version_id_ = buffer.Get<uint16_t>().get();
  correlation_id_ = buffer.Get<uint32_t>().get();
  cafca::util::debug("correlation_id: ", correlation_id_);
  client_id_ = buffer.Get<cafca::util::ShortString>().get_value_or(cafca::util::ShortString { "" }).value;
  cafca::util::debug("client_id: ", client_id_);
  replica_id_ = buffer.Get<int32_t>().get();
  cafca::util::debug("replica_id: ", replica_id_);
  const auto topicCount = buffer.Get<uint32_t>().get();
  cafca::util::debug("topic_count: ", topicCount);
  request_info_ = std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<int64_t, uint32_t>>();
  for (uint32_t i = 0; i < topicCount; ++i) {
    const auto topic = buffer.Get<cafca::util::ShortString>().get().value;
    cafca::util::debug("topic: ", topic);
    const auto partitionCount = buffer.Get<uint32_t>().get();
    for (uint32_t j = 0; j < partitionCount; ++j) {
      const auto partitionId = buffer.Get<uint32_t>().get();
      cafca::util::debug("parition_id: ", partitionId);
      const auto time = buffer.Get<int64_t>().get();
      cafca::util::debug("time: ", time);
      const auto maxNumOffsets = buffer.Get<uint32_t>().get();
      cafca::util::debug("max_num_offsets: ", maxNumOffsets);
      const auto key = cafca::server::cluster::TopicAndPartition { topic, partitionId };
      request_info_.insert(std::make_pair(key, std::make_pair(time, maxNumOffsets)));
    }
  }
}

}
}
}
}
