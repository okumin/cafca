#ifndef CAFCA_PRODUCER_REQUEST_H
#define CAFCA_PRODUCER_REQUEST_H

#include <stdint.h>
#include <string>
#include <map>
#include <cafca/server/cluster/topic_and_partition.h>
#include <cafca/message/messages_buffer.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

class ProducerRequest {
 public:
  using Messages = std::vector<std::tuple<cafca::server::cluster::TopicAndPartition, cafca::message::MessagesBuffer>>;

  ProducerRequest(const ProducerRequest&) = delete;
  ProducerRequest& operator=(const ProducerRequest&) = delete;
  ProducerRequest(ProducerRequest&&) = default;
  ProducerRequest& operator=(ProducerRequest&&) = default;

  ProducerRequest(cafca::util::ByteBuffer &buffer);

  const uint32_t correlation_id() const {
    return correlation_id_;
  }

  Messages messages() const {
    return messages_;
  }

  const uint16_t required_acks() const {
    return required_acks_;
  }

 private:
  uint16_t version_id_;
  uint32_t correlation_id_;
  std::string client_id_;
  uint16_t required_acks_;
  uint32_t ack_timeout_ms_;
  Messages messages_;
};

}
}
}
}

#endif //CAFCA_PRODUCER_REQUEST_H
