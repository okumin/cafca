#include "message.h"

namespace cafca {
namespace message {

Message::Message(const cafca::util::ByteBuffer &buffer) : buffer_(buffer) {
  assert(size() >= min_header_size);
}

}
}
