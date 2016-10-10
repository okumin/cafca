#ifndef CAFCA_FILE_MESSAGE_SET_H
#define CAFCA_FILE_MESSAGE_SET_H

#include <stdint.h>
#include <cafca/util/file_channel.h>
namespace cafca {
namespace message {

class FileMessageSet {
 public:
  FileMessageSet() = default;
  FileMessageSet(const FileMessageSet&) = delete;
  FileMessageSet& operator=(const FileMessageSet&) = delete;
  FileMessageSet(FileMessageSet&&) = delete;
  FileMessageSet& operator=(FileMessageSet&&) = delete;

  virtual const uint32_t size() const = 0;
  virtual void Read(cafca::util::ByteBuffer &buffer) = 0;

  virtual ~FileMessageSet() = default;
};

class FSMessageSet : public FileMessageSet {
 public:
  FSMessageSet(const std::shared_ptr<cafca::util::FileChannel> channel,
               const uint32_t position,
               const uint32_t size) : channel_(channel), position_(position), size_(size) {}

  virtual const uint32_t size() const override {
    return size_;
  }

  virtual void Read(cafca::util::ByteBuffer &buffer) override {
    channel_->Read(buffer, position_, size_);
  }

  virtual ~FSMessageSet() = default;

 private:
  const uint32_t position_;
  const uint32_t size_;
  std::shared_ptr<cafca::util::FileChannel> channel_;
};

class NullMessageSet : public FileMessageSet {
 public:
  virtual const uint32_t size() const override {
    return 0;
  }
  virtual void Read(cafca::util::ByteBuffer &buffer) override { }
  virtual ~NullMessageSet() = default;
};

}
}

#endif //CAFCA_FILE_MESSAGE_SET_H
