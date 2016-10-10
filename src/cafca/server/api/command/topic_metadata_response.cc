#include "topic_metadata_response.h"

namespace cafca {
namespace server {
namespace api {
namespace command {

cafca::util::ByteBuffer TopicMetadataResponse::to_bytes() const {
  uint32_t size = 4 + 4;
  for (const auto &broker : brokers) {
    size += broker.size_in_bytes();
  }
  size += 4;
  for (const auto &metadata : topics_metadata) {
    size += metadata.size_in_bytes();
  }
  auto buffer = util::ByteBuffer(size + 4);
  buffer.Put(size);
  buffer.Put(*this);
  buffer.Rewind();
  return buffer;
}

}
}
}
}
