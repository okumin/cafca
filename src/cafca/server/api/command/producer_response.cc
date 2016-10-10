#include <map>
#include "producer_response.h"
#include <cafca/util/short_string.h>

namespace cafca {
namespace server {
namespace api {
namespace command {

using cafca::server::cluster::TopicAndPartition;
using cafca::util::ByteBuffer;
using cafca::util::ShortString;

ProducerResponse::ProducerResponse(const uint32_t correlation_id,
                                   const Statuses &statuses)
    : correlation_id_(correlation_id), statuses_(statuses) {}

cafca::util::ByteBuffer ProducerResponse::to_bytes() const {
  auto grouped = std::map<std::string, std::vector<std::tuple<TopicAndPartition, ProducerResponseStatus>>>();
  for (auto &status : statuses_) {
    auto topic = std::get<0>(status).topic;
    if (grouped.find(topic) == grouped.end()) {
      grouped[topic] = std::vector<std::tuple<TopicAndPartition, ProducerResponseStatus>>();
    }
    auto &vector = grouped[topic];
    vector.push_back(status);
  }

  uint32_t size = 4 + 4; // correlation id and topic count
  for (auto &topic_and_status : grouped) {
    size += cafca::util::short_string_length(std::get<0>(topic_and_status));
    size += 4; // partition count
    // partition count * (partition id + error code + offset)
    size += std::get<1>(topic_and_status).size() * (4 + 2 + 8);
  }
  size += 4; // throttle time size

  ByteBuffer buffer = ByteBuffer(size + 4);
  buffer.Put(size);
  buffer.Put<uint32_t>(correlation_id_);
  buffer.Put<uint32_t>(static_cast<uint32_t>(grouped.size()));

  for (auto &topic_and_status : grouped) {
    buffer.Put<ShortString>(ShortString{std::get<0>(topic_and_status)});
    const auto statuses = std::get<1>(topic_and_status);
    buffer.Put<uint32_t>(static_cast<uint32_t>(statuses.size()));
    for (auto &status : statuses) {
      const TopicAndPartition tp = std::get<0>(status);
      const ProducerResponseStatus prs = std::get<1>(status);
      buffer.Put<uint32_t>(tp.partition);
      buffer.Put<uint16_t>(prs.error);
      buffer.Put<uint64_t>(prs.offset);
    }
  }
  buffer.Put<uint32_t>(0); // throttle time
  buffer.Rewind();

  return buffer;
}

}
}
}
}
