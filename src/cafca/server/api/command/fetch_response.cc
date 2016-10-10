#include <unordered_map>
#include <cafca/util/short_string.h>
#include "fetch_response.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

cafca::util::ByteBuffer FetchResponse::to_bytes() const {
  auto byTopic = std::unordered_map<std::string, std::vector<std::pair<uint32_t, FetchResponsePartitionData>>>();
  for (auto v : data_) {
    if (byTopic.find(v.first.topic) == byTopic.end()) {
      auto list = std::vector<std::pair<uint32_t, FetchResponsePartitionData>>();
      list.push_back(std::make_pair(v.first.partition, v.second));
      byTopic.insert(std::make_pair(v.first.topic, list));
    } else {
      auto list = byTopic.at(v.first.topic);
      list.push_back(std::make_pair(v.first.partition, v.second));
    }
  }

  uint32_t size = 0;
  if (version_ > 0) size += 4;
  size += 4 + 4;

  for (auto v : byTopic) {
    size += cafca::util::short_string_length(v.first);
    size += 4;
    for (auto v2 : v.second) {
      size += 4 + 2 + 8 + 4 + v2.second.messages->size();
    }
  }

  auto buffer = cafca::util::ByteBuffer(4 + size);
  buffer.Put(size);
  buffer.Put(correlation_id_);
  if (version_ > 0) {
    buffer.Put<uint32_t>(0);
  }
  buffer.Put(static_cast<uint32_t>(byTopic.size()));

  for (auto v : byTopic) {
    buffer.Put(cafca::util::ShortString { v.first });
    buffer.Put(static_cast<uint32_t>(v.second.size()));
    for (auto v2 : v.second) {
      buffer.Put(v2.first);
      auto data = v2.second;
      buffer.Put(data.error);
      buffer.Put<int64_t>(-1);
      buffer.Put(static_cast<uint32_t>(data.messages->size()));
      data.messages->Read(buffer);
    }
  }

  buffer.Rewind();
  return buffer;
}

}
}
}
}
