#ifndef CAFCA_FETCH_RESPONSE_H
#define CAFCA_FETCH_RESPONSE_H

#include <cafca/message/file_message_set.h>
#include <cafca/server/cluster/topic_and_partition.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

struct FetchResponsePartitionData {
  const uint16_t  error;
  std::shared_ptr<cafca::message::FileMessageSet> messages;
};

class FetchResponse {
 public:
  FetchResponse(const uint32_t correlationId,
                const std::vector<std::pair<cafca::server::cluster::TopicAndPartition, FetchResponsePartitionData>> &data,
                const uint32_t version)
      : correlation_id_(correlationId), data_(data), version_(version) {}

  FetchResponse(const FetchResponse&) = delete;
  FetchResponse& operator=(const FetchResponse&) = delete;
  FetchResponse(FetchResponse&&) = default;
  FetchResponse& operator=(FetchResponse&&) = default;

  cafca::util::ByteBuffer to_bytes() const;

 private:
  const uint32_t correlation_id_;
  const std::vector<std::pair<cafca::server::cluster::TopicAndPartition, FetchResponsePartitionData>> data_;
  const uint32_t version_;
};

}
}
}
}

#endif //CAFCA_FETCH_RESPONSE_H
