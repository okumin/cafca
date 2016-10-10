#include <memory>
#include <cafca/server/cluster/topic_and_partition.h>
#include "cafca_controller.h"

namespace cafca {
namespace server {

using cafca::server::api::command::ProducerResponseStatus;

cafca::server::cluster::PartitionMembers partitionMembers(const std::string &host) {
  std::unordered_set<uint32_t> replicas;
  replicas.insert(cafca::server::kBrokerId);
  return cafca::server::cluster::PartitionMembers {
      cafca::server::cluster::ActiveMembers {
          cafca::server::kBrokerId,
          0,
          cafca::server::kIsr
      },
      0,
      replicas
  };
}

CafcaController::CafcaController(): endpoint_(cafca::server::cluster::BrokerEndpoint {cafca::server::kBrokerId, "127.0.0.1", 9092}),
                                    members_(partitionMembers("127.0.0.1")) {
  std::unordered_set<uint32_t> testPartitions;
  testPartitions.insert(1);
  // default topic
  partitions_ids_["test"] = testPartitions;
  const auto key = cluster::TopicAndPartition { "test", 1 };
  partitions_[key] = std::make_shared<cluster::Partition>(key, 1);
}

const std::vector<cafca::server::cluster::TopicMetadata> CafcaController::GetTopicMetadata(const std::unordered_set<std::string> &topics) const {
  std::vector<cafca::server::cluster::TopicMetadata> responses;
  if (topics.empty()) {
    for (auto &kv: partitions_ids_) {
      responses.push_back(GetTopic(kv.first, kv.second));
    }
  } else {
    for (auto &topic: topics) {
      const auto partitions = partitions_ids_.find(topic);
      if (partitions != partitions_ids_.end()) {
        responses.push_back(GetTopic(topic, partitions->second));
      }
    }
  }
  return responses;
}

CafcaController::AppendResults CafcaController::Append(Messages &messages) {
  AppendResults results = AppendResults();

  for (auto &kv : messages) {
    const auto belongsTo = std::get<0>(kv);
    auto messagesBuffer = std::get<1>(kv);
    const auto partition = get_partition(belongsTo);
    if (!partition) throw std::runtime_error("No partition is found.");

    const auto info = partition.get()->Append(messagesBuffer);
    results.push_back(std::make_tuple(
        belongsTo,
        ProducerResponseStatus{0, info.first_offset}
    ));
  }

  return results;
}

const std::vector<uint64_t> CafcaController::FetchOffsets(const cafca::server::cluster::TopicAndPartition &topicAndPartition,
                                                          const int64_t time,
                                                          const uint32_t maxNumOffsets) {
  auto partition = get_partition(topicAndPartition);
  if (partition) {
    return partition.get()->FetchOffsetsBefore(time, maxNumOffsets);
  } else {
    if (time == -1 || time == -2) {
      return std::vector<uint64_t>(1, 0);
    } else {
      return std::vector<uint64_t>(0);
    }
  }
}

const std::unordered_map<cafca::server::cluster::TopicAndPartition, LogReadResult> CafcaController::FetchMessages(const std::unordered_map<
    cafca::server::cluster::TopicAndPartition,
    cafca::server::api::command::PartitionFetchInfo> &partitions) {
  auto result = std::unordered_map<cafca::server::cluster::TopicAndPartition, LogReadResult>();
  for (auto kv : partitions) {
    auto partition = get_partition(kv.first);
    if (partition) {
      const auto logEndOffset = partition.get()->log_end_offset();
      const auto info = partition.get()->Read(kv.second.offset, kv.second.fetch_size);
      const auto readToEndOfLog = logEndOffset - info.metadata.message_offset <= 0;
      result.insert(std::make_pair(kv.first, LogReadResult { info, kv.second.fetch_size, readToEndOfLog }));
    } else {
      throw std::runtime_error("TODO: empty response");
    }
  }
  return result;
}

const cafca::server::cluster::TopicMetadata CafcaController::GetTopic(const std::string &topic,
                                                                      const std::unordered_set<uint32_t> partitions) const {
  std::vector<cafca::server::cluster::PartitionMetadata> metadata;
  for (const auto &partition_id : partitions) {
    std::vector<cafca::server::cluster::BrokerEndpoint> replicaEndpoints;
    replicaEndpoints.push_back(endpoint_);

    std::vector<cafca::server::cluster::BrokerEndpoint> isr_endpoints;

    metadata.push_back(cafca::server::cluster::PartitionMetadata {
        partition_id,
        endpoint_,
        replicaEndpoints,
        isr_endpoints,
        0
    });
  }

  return cafca::server::cluster::TopicMetadata {
      topic,
      metadata,
      0
  };
}

}
}
