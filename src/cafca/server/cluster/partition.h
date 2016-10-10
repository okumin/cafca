#ifndef CAFCA_PARTITION_H
#define CAFCA_PARTITION_H

#include <boost/optional/optional.hpp>
#include <cafca/commit_log/commit_log.h>
#include <unordered_map>
#include "topic_and_partition.h"
#include "replica.h"
namespace cafca {
namespace server {
namespace cluster {

class Partition : boost::noncopyable {
 public:
  Partition(Partition&& that) = default;

  Partition(TopicAndPartition belongs_to, const int local_broker_id) : belongs_to_(belongs_to), local_broker_id_(local_broker_id) {
    leader_replica_id_ = local_broker_id;
    replicas_[local_broker_id] = std::make_shared<LocalReplica>(
        local_broker_id,
        belongs_to,
        "/tmp/mofu/" + belongs_to.topic +  "-" + std::to_string(belongs_to.partition) + "/"
    );
  }

  const cafca::commit_log::LogAppendInfo Append(cafca::message::MessagesBuffer &buffer);

  const std::vector<uint64_t> FetchOffsetsBefore(const int64_t time, const uint32_t maxOffsetsNum);

  const cafca::commit_log::FetchInfo Read(const uint64_t startOffset, const uint32_t maxLength);

  const uint64_t log_end_offset() const;

 private:
  std::shared_ptr<cafca::server::cluster::Replica> get_leader() const;

  const TopicAndPartition belongs_to_;
  const int local_broker_id_;
  boost::optional<int> leader_replica_id_ = boost::none;
  std::unordered_map<int, std::shared_ptr<Replica>> replicas_;
};

}
}
}

#endif //CAFCA_PARTITION_H
