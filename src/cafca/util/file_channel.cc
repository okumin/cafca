#include "file_channel.h"
#include <boost/filesystem/operations.hpp>

namespace cafca {
namespace util {

FileChannel::FileChannel(const std::string &filename, const uint32_t size): path_(filename) {
  file_ = std::unique_ptr<std::fstream>(new std::fstream());
  file_->open(filename, std::ios::in | std::ios::app);
}

size_t FileChannel::Write(ByteBuffer buffer) {
  const auto size = buffer.remaining();
  const auto contents = reinterpret_cast<char*>(buffer.pointer());
  file_->write(contents, size).flush();
  return size;
}

void FileChannel::Read(ByteBuffer &buffer, const uint32_t position, const uint32_t size) {
  if (size - position == 0) return;
  file_->seekg(position);
  file_->read(reinterpret_cast<char*>(buffer.pointer()), size);
}

const uint64_t FileChannel::GetLastModifiedTime() {
  return static_cast<uint64_t>(boost::filesystem::last_write_time(path_));
}

const uint64_t FileChannel::Size() {
  return boost::filesystem::file_size(path_);
}

}
}
