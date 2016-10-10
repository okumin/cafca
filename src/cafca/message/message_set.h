#ifndef CAFCA_MESSAGE_SET_H
#define CAFCA_MESSAGE_SET_H

#include <cafca/util/byte_buffer.h>
#include "message.h"
namespace cafca {
namespace message {

class MessageIterator;

class MessageSet {
 public:
  friend class MessageIterator;

  using iterator = MessageIterator;

  explicit MessageSet(cafca::util::ByteBuffer &messages);

  cafca::util::ByteBuffer buffer() const {
    return buffer_;
  }

  const uint32_t size_in_bytes() const {
    return buffer_.limit();
  }

  MessageSet::iterator begin();

  MessageSet::iterator end();


 private:
  cafca::util::ByteBuffer buffer_;
};

class MessageIterator : public std::iterator<std::input_iterator_tag, Message> {
  friend class MessageSet;

 public:
  MessageIterator() = delete;

  MessageIterator(const MessageIterator &iterator);

  const Message& operator*() const;

  MessageIterator& operator++();

  MessageIterator operator++(int);

  const bool operator==(const MessageIterator &it) const;

  const bool operator!=(const MessageIterator &it) const;

 private:
  MessageIterator(MessageSet * const messages, const uint32_t position);

  MessageSet * const messages_;
  uint32_t position_;
};

}
}

#endif //CAFCA_MESSAGE_SET_H
