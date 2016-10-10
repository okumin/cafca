#include "topic_metadata_request.h"
#include <cafca/util/logging.h>
#include <cafca/util/short_string.h>

namespace cafca {
namespace server {
namespace api {
namespace command {

using cafca::util::ShortString;

TopicMetadataRequest::TopicMetadataRequest(cafca::util::ByteBuffer &buffer) {
  version_id_ = buffer.Get<uint16_t>().get();
  correlation_id_ = buffer.Get<uint32_t>().get();
  client_id_ = buffer.Get<ShortString>().get().value;
  const auto num_topics = buffer.Get<uint32_t>().get();
  cafca::util::debug("version: ", version_id_);
  cafca::util::debug("correlation_id: ", correlation_id_);
  cafca::util::debug("client_id: ", client_id_);
  for (int i = 0; i < num_topics; ++i) {
    const auto topic = buffer.Get<ShortString>().get().value;
    cafca::util::debug("topic: ", topic);
    topics_.push_back(topic);
  }
}

}
}
}
}
