#ifndef CAFCA_FILE_CHANNEL_H
#define CAFCA_FILE_CHANNEL_H

#include <string>
#include <stdint.h>
#include <fstream>
#include "byte_buffer.h"
namespace cafca {
namespace util {

class FileChannel : boost::noncopyable {
 public:
  FileChannel(const std::string &filename, const uint32_t size);

  size_t Write(ByteBuffer buffer);

  /**
   * Reads this file from the given position and returns T.
   */
  template <typename T> const boost::optional<T> Read(const uint32_t position) {
    file_->seekg(position);
    ByteBuffer buf = ByteBuffer(static_cast<uint32_t>(sizeof(T)));
    file_->read(reinterpret_cast<char*>(buf.pointer()), sizeof(T));
    return buf.Get<T>();
  }

  // TODO: Implement a new API using sendfile
  void Read(ByteBuffer &buffer, const uint32_t position, uint32_t size);

  void Truncate(const uint32_t size) {
    // TODO
  }

  void Position(const uint32_t position) {
    // TODO
  }

  const uint64_t GetLastModifiedTime();

  const uint64_t Size();

 private:
  const std::string path_;
  std::unique_ptr<std::fstream> file_;
};

}
}

#endif //CAFCA_FILE_CHANNEL_H
