#ifndef CAFCA_CAFCA_CONTROLLER_H
#define CAFCA_CAFCA_CONTROLLER_H

#include "cafca/server/cluster/broker_endpoint.h"
#include "cafca/server/cluster/topic_metadata.h"
#include <unordered_set>
#include <cafca/server/cluster/partition_members.h>
#include <unordered_map>
#include <cafca/commit_log/commit_log.h>
#include <cafca/server/cluster/partition.h>
#include <cafca/server/api/command/producer_request.h>
#include <cafca/server/api/command/producer_response.h>
#include <cafca/server/api/command/partition_fetch_info.h>
namespace cafca {
namespace server {

struct LogReadResult {
  const cafca::commit_log::FetchInfo info;
  const uint32_t read_size;
  const bool is_read_from_log_end;
};

class CafcaController {
 public:
  using AppendResults = std::vector<std::tuple<cluster::TopicAndPartition, cafca::server::api::command::ProducerResponseStatus>>;
  using Messages = cafca::server::api::command::ProducerRequest::Messages;

  CafcaController();

  CafcaController(const CafcaController&) = delete;
  CafcaController& operator=(const CafcaController&) = delete;
  CafcaController(CafcaController&&) = delete;
  CafcaController& operator=(CafcaController&&) = delete;

  const std::vector<cafca::server::cluster::BrokerEndpoint> GetAliveBrokers() const {
    std::vector<cafca::server::cluster::BrokerEndpoint> brokers;
    brokers.push_back(endpoint_);
    return brokers;
  }

  const std::vector<cafca::server::cluster::TopicMetadata> GetTopicMetadata(const std::unordered_set<std::string> &topics) const;

  AppendResults Append(Messages &messages);

  const std::vector<uint64_t> FetchOffsets(const cafca::server::cluster::TopicAndPartition& topicAndPartition, const int64_t time, const uint32_t maxNumOffsets);

  const std::unordered_map<cafca::server::cluster::TopicAndPartition, LogReadResult> FetchMessages(const std::unordered_map<cafca::server::cluster::TopicAndPartition, cafca::server::api::command::PartitionFetchInfo> &partitions);

 private:
  cafca::server::cluster::BrokerEndpoint endpoint_;
  cafca::server::cluster::PartitionMembers members_;

  // topic name -> partition ids
  std::unordered_map<std::string, std::unordered_set<uint32_t>> partitions_ids_;

  std::unordered_map<cluster::TopicAndPartition, std::shared_ptr<cluster::Partition>> partitions_;

  const cafca::server::cluster::TopicMetadata GetTopic(const std::string &topic, const std::unordered_set<uint32_t> partitions) const;

  const boost::optional<std::shared_ptr<cluster::Partition>> get_partition(const cluster::TopicAndPartition &belongs_to) {
    const auto itr = partitions_.find(belongs_to);
    if (itr == partitions_.end()) return boost::none;
    return itr->second;
  }
};

static const uint32_t kBrokerId = 1;
static const std::vector<uint32_t> kIsr;

}
}

#endif //CAFCA_CAFCA_CONTROLLER_H
