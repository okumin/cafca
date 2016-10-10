#ifndef CAFCA_TOPIC_METADATA_REQUEST_H
#define CAFCA_TOPIC_METADATA_REQUEST_H

#include <stdint.h>
#include <string>
#include <vector>
#include <cafca/util/byte_buffer.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

class TopicMetadataRequest {
 public:
  TopicMetadataRequest(cafca::util::ByteBuffer &buffer);

  TopicMetadataRequest(const TopicMetadataRequest&) = delete;
  TopicMetadataRequest& operator=(const TopicMetadataRequest&) = delete;
  TopicMetadataRequest(TopicMetadataRequest&&) = default;
  TopicMetadataRequest& operator=(TopicMetadataRequest&&) = default;

  const uint32_t correlation_id() const {
    return correlation_id_;
  }

  const std::vector<std::string> topics() const {
    return topics_;
  }

 private:
  uint16_t version_id_;
  uint32_t correlation_id_;
  std::string client_id_;
  std::vector<std::string> topics_;
};

}
}
}
}

#endif //CAFCA_TOPIC_METADATA_REQUEST_H
