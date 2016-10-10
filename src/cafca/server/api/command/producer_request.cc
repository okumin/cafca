#include <cafca/util/logging.h>
#include "producer_request.h"
#include "cafca/util/short_string.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

using cafca::server::cluster::TopicAndPartition;
using cafca::message::MessagesBuffer;
using cafca::util::ShortString;

ProducerRequest::ProducerRequest(cafca::util::ByteBuffer &buffer) {
  version_id_ = buffer.Get<uint16_t>().get();
  correlation_id_ = buffer.Get<uint32_t>().get();
  cafca::util::debug("correlation_id: ", correlation_id_);
  client_id_ = buffer.Get<ShortString>().get().value;
  cafca::util::debug("client_id: ", client_id_);
  required_acks_ = buffer.Get<uint16_t>().get();
  cafca::util::debug("required_acks: ", required_acks_);
  ack_timeout_ms_ = buffer.Get<uint32_t>().get();
  const auto topic_count = buffer.Get<uint32_t>().get();

  for (int ti = 0; ti < topic_count; ++ti) {
    const auto topic = buffer.Get<ShortString>().get().value;
    const auto partition_count = buffer.Get<uint32_t>().get();
    cafca::util::debug("topic: ", topic);
    for (int pi = 0; pi < partition_count; ++pi) {
      const auto partition = buffer.Get<uint32_t>().get();
      cafca::util::debug("partition: ", partition);
      const auto message_set_size = buffer.Get<uint32_t>().get();
      cafca::util::debug("message_size: ", message_set_size);

      messages_.push_back(std::make_tuple(
          TopicAndPartition{topic, partition},
          MessagesBuffer(buffer.Slice(buffer.position(), message_set_size))
      ));
      buffer.Skip(message_set_size);
    }
  }
}

}
}
}
}
