#include <cafca/util/short_string.h>
#include "offset_response.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

OffsetResponse::OffsetResponse(const uint32_t correlation_id,
                               const std::unordered_map<cafca::server::cluster::TopicAndPartition,
                                                        std::pair<uint16_t,
                                                                  std::vector<uint64_t>>> &partition_error_and_offsets)
    : correlation_id_(correlation_id), partition_error_and_offsets_(partition_error_and_offsets) {}

cafca::util::ByteBuffer OffsetResponse::to_bytes() const {
  uint32_t size = 0;
  size += 4 + 4;

  auto byTopic = std::unordered_map<std::string, std::vector<std::pair<uint32_t, std::pair<uint16_t, std::vector<uint64_t>>>>>();
  for (auto kv : partition_error_and_offsets_) {
    const auto topic = kv.first.topic;
    const auto partition = kv.first.partition;
    const auto error = kv.second.first;
    const auto offsets = kv.second.second;
    if (byTopic.find(topic) == byTopic.end()) {
      auto results = std::vector<std::pair<uint32_t, std::pair<uint16_t, std::vector<uint64_t>>>>();
      results.push_back(std::make_pair(partition, std::make_pair(error, offsets)));
      byTopic.insert(std::make_pair(topic, results));
    } else {
      auto results = byTopic.at(topic);
      results.push_back(std::make_pair(partition, std::make_pair(error, offsets)));
    }
  }

  for (auto v : byTopic) {
    size += cafca::util::short_string_length(v.first);
    size += 4;
    for (auto v2 : v.second) {
      size += 4 + 2 + 4;
      size += v2.second.second.size() * 8;
    }
  }

  auto buffer = cafca::util::ByteBuffer(size + 4);
  buffer.Put(size);
  buffer.Put(correlation_id_);
  buffer.Put<uint32_t>(static_cast<uint32_t>(byTopic.size()));
  for (auto v : byTopic) {
    buffer.Put(cafca::util::ShortString { v.first });
    buffer.Put(static_cast<uint32_t>(v.second.size()));
    for (auto v2 : v.second) {
      buffer.Put(v2.first);
      buffer.Put(v2.second.first);
      buffer.Put(static_cast<uint32_t>(v2.second.second.size()));
      for (auto offset : v2.second.second) {
        buffer.Put(offset);
      }
    }
  }
  buffer.Rewind();
  return buffer;
}

}
}
}
}
