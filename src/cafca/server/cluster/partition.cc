#include "partition.h"

namespace cafca {
namespace server {
namespace cluster {

const cafca::commit_log::LogAppendInfo Partition::Append(cafca::message::MessagesBuffer &buffer) {
  return get_leader()->Append(buffer);
}

const std::vector<uint64_t> Partition::FetchOffsetsBefore(const int64_t time, const uint32_t maxOffsetsNum) {
  return get_leader()->FetchOffsetsBefore(time, maxOffsetsNum);
}

const cafca::commit_log::FetchInfo Partition::Read(const uint64_t startOffset, const uint32_t maxLength) {
  return get_leader()->Read(startOffset, maxLength);
}

const uint64_t Partition::log_end_offset() const {
  return get_leader()->log_end_offset();
}

std::shared_ptr<cafca::server::cluster::Replica> Partition::get_leader() const {
  if (!leader_replica_id_) throw std::runtime_error("The leader is not found.");
  const auto itr = replicas_.find(leader_replica_id_.get());
  if (itr == replicas_.end()) throw std::runtime_error("The replica is not found.");
  return itr->second;
}

}
}
}
