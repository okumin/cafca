#ifndef CAFCA_MESSAGEBUFFER_H
#define CAFCA_MESSAGEBUFFER_H

#include <cafca/util/byte_buffer.h>
#include "message.h"
#include "message_set.h"
namespace cafca {
namespace message {

/**
 * Messages without offset assignment.
 */
class MessagesBuffer {
 public:
  explicit MessagesBuffer(const cafca::util::ByteBuffer &buffer);

  /**
   * Assigns the given offsets to messages_.
   */
  const std::tuple<uint64_t, cafca::message::MessageSet> AssignOffsets(uint64_t offset_counter);

 private:
  cafca::util::ByteBuffer buffer_;
};

}
}

#endif //CAFCA_MESSAGEBUFFER_H
