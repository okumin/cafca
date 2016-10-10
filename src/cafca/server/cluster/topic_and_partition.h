#ifndef CAFCA_TOPIC_AND_PARTITION_H
#define CAFCA_TOPIC_AND_PARTITION_H

#include <stdint.h>
#include <string>
namespace cafca {
namespace server {
namespace cluster {

struct TopicAndPartition {
  const std::string topic;
  const uint32_t partition;

  const bool operator==(const TopicAndPartition &other) const {
    return topic == other.topic && partition == other.partition;
  }

  const bool operator!=(const TopicAndPartition &other) const {
    return !operator==(other);
  }

};

}
}
}

namespace std {

template <>
class hash<cafca::server::cluster::TopicAndPartition> {
 public:
  std::size_t operator ()(const cafca::server::cluster::TopicAndPartition &key) const {
    return std::hash<std::string>()(key.topic) * 31 + key.partition;
  }
};

}

#endif //CAFCA_TOPIC_AND_PARTITION_H
