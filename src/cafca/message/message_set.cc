#include "message_set.h"

namespace cafca {
namespace message {

MessageSet::MessageSet(cafca::util::ByteBuffer &messages) : buffer_(messages) {}

MessageSet::iterator MessageSet::begin() {
  return MessageIterator(this, 0);
}

MessageSet::iterator MessageSet::end() {
  return MessageIterator(this, this->size_in_bytes());
}

MessageIterator::MessageIterator(const MessageIterator &iterator)
    : messages_(iterator.messages_), position_(iterator.position_) {}

const Message& MessageIterator::operator*() const {
  assert(position_ < messages_->size_in_bytes());
  const auto value = messages_->buffer().Get<Message>(position_);
  if (!value) throw std::runtime_error("This iterator is in invaild state.");
  return value.get();
}

MessageIterator& MessageIterator::operator++() {
  const auto size = messages_->buffer().Get<uint32_t>(position_ + Message::offset_length);
  if (!size) throw std::runtime_error("This iterator is in invaild state.");
  position_ += Message::offset_length + Message::message_size_length + size.get();
  return *this;
}

MessageIterator MessageIterator::operator++(int) {
  const auto result = *this;
  const auto size = messages_->buffer().Get<uint32_t>(position_ + Message::offset_length);
  if (!size) throw std::runtime_error("This iterator is in invaild state.");
  position_ += Message::offset_length + Message::message_size_length + size.get();
  return result;
}

const bool MessageIterator::operator==(const MessageIterator &it) const {
  return this->messages_ == it.messages_ && this->position_ == it.position_;
}

const bool MessageIterator::operator!=(const MessageIterator &it) const {
  return !(*this == it);
}

MessageIterator::MessageIterator(MessageSet * const messages,
                                 const uint32_t position)
    : messages_(messages), position_(position) {}

}
}
