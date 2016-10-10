#ifndef CAFCA_REPLICA_H
#define CAFCA_REPLICA_H

#include <cafca/commit_log/commit_log.h>
#include "topic_and_partition.h"
namespace cafca {
namespace server {
namespace cluster {

class Replica : boost::noncopyable {
 public:
  Replica(const int broker_id) : broker_id_(broker_id) {}

  virtual const cafca::commit_log::LogAppendInfo Append(cafca::message::MessagesBuffer &buffer) {
    throw std::runtime_error("Replica never supports Append.");
  }

  virtual const std::vector<uint64_t> FetchOffsetsBefore(const int64_t time, const uint32_t maxNumOffsets) {
    throw std::runtime_error("Replica never supports FetchOffsetsBefore.");
  }

  virtual const cafca::commit_log::FetchInfo Read(const uint64_t startOffset, const uint32_t maxLength) {
    throw std::runtime_error("Replica never supports FetchOffsetsBefore.");
  }

  virtual const uint64_t log_end_offset() const {
    throw std::runtime_error("Replica never supports log_end_offset.");
  }

 protected:
  const int broker_id_;
};

class LocalReplica : public Replica {
 public:
  explicit LocalReplica(const int broker_id, const TopicAndPartition &belong_to, const std::string &dir)
      : Replica(broker_id),
        log_(std::unique_ptr<cafca::commit_log::CommitLog>(new cafca::commit_log::CommitLog(belong_to.topic, belong_to.partition, dir))) {
  }

  LocalReplica(LocalReplica&& that) = default;

  virtual const cafca::commit_log::LogAppendInfo Append(cafca::message::MessagesBuffer &buffer) override {
    // TODO: replicaManager.tryCompleteDelayedFetch
    return log_->Append(buffer);
  }

  virtual const std::vector<uint64_t> FetchOffsetsBefore(const int64_t time,
                                                                       const uint32_t maxNumOffsets) override {
    return log_->FetchOffsetsBefore(time, maxNumOffsets);
  }

  virtual const cafca::commit_log::FetchInfo Read(const uint64_t startOffset,
                                                                const uint32_t maxLength) override {
    return log_->Read(startOffset, maxLength);
  }

  virtual const uint64_t log_end_offset() const override {
    return log_->log_end_offset();
  }

 private:
  std::unique_ptr<cafca::commit_log::CommitLog> log_;
};

}
}
}

#endif //CAFCA_REPLICA_H
