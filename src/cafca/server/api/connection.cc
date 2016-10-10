#include <cafca/util/byte_buffer.h>
#include <cafca/server/api/command/topic_metadata_request.h>
#include <cafca/server/api/command/topic_metadata_response.h>
#include <cafca/server/api/command/offset_request.h>
#include <cafca/server/api/command/offset_response.h>
#include <cafca/server/api/command/fetch_request.h>
#include <cafca/server/api/command/fetch_response.h>
#include <cafca/util/logging.h>
#include "connection.h"

namespace cafca {
namespace server {
namespace api {

using boost::asio::ip::tcp;
using cafca::server::CafcaComponents;
using cafca::util::ByteBuffer;

Connection::Connection(tcp::socket &socket,
                       const std::shared_ptr<CafcaComponents> components)
    : socket_(std::move(socket)), components_(components) {}

void Connection::Start() {
  cafca::util::debug("Connection started");
  ReadSize();
}

void Connection::ReadSize() {
  const auto self(shared_from_this());
  const auto size_buffer = std::make_shared<ByteBuffer>(4);
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(size_buffer->pointer(0), sizeof(uint32_t)),
      [this, self, size_buffer](boost::system::error_code ec, std::size_t) {
        if (ec.value() == 2) {
          return;
        }
        if (ec) {
          cafca::util::debug("error(", ec.value(), "): ", ec.message());
          return;
        }
        const auto size = size_buffer->Get<uint32_t>().get();
        cafca::util::debug("Read a size: ", size);
        ReadPayload(size);
      }
  );
}

void Connection::ReadPayload(const uint32_t size) {
  const auto self(shared_from_this());
  const auto buffer = std::make_shared<ByteBuffer>(size);
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(buffer->pointer(0), size),
      [this, self, buffer](boost::system::error_code ec, std::size_t) {
        if (ec) throw std::runtime_error("Can't read a payload from the socket.");
        const auto request_id = buffer->Get<uint16_t>().get();
        cafca::util::debug("Read a request: ", request_id);
        switch (request_id) {
          case 0 : {
            const auto request = command::ProducerRequest(*buffer.get());
            auto messages = request.messages();
            const auto result = components_->controller.Append(messages);
            if (request.required_acks() == 0) {
              ReadSize();
            } else {
              const auto response = command::ProducerResponse(
                  request.correlation_id(),
                  result
              );
              const auto bytes = std::make_shared<ByteBuffer>(response.to_bytes());
              boost::asio::async_write(
                  socket_,
                  boost::asio::buffer(bytes->pointer(0), bytes->limit()),
                  [this, self, bytes](boost::system::error_code wec, std::size_t) {
                    if (wec) throw std::runtime_error("Can't send a producer response.");
                    ReadSize();
                  }
              );
            }
            break;
          }
          case 1 : {
            const auto request = command::FetchRequest(*buffer.get());
            const auto results = components_->controller.FetchMessages(request.info());
            auto data = std::vector<std::pair<cafca::server::cluster::TopicAndPartition, command::FetchResponsePartitionData>>();
            for (auto result : results) {
              data.push_back(std::make_pair(result.first, command::FetchResponsePartitionData { 0, result.second.info.messages }));
            }
            const auto response = command::FetchResponse(request.correlation_id(), data, request.version_id());
            const auto bytes = std::make_shared<ByteBuffer>(response.to_bytes());
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(bytes->pointer(0), bytes->limit()),
                [this, self, bytes](boost::system::error_code wec, std::size_t) {
                  if (wec) throw std::runtime_error("Can't send a fetch response.");
                  ReadSize();
                }
            );
            break;
          }
          case 2 : {
            const auto request = command::OffsetRequest(*buffer.get());
            auto map = std::unordered_map<cafca::server::cluster::TopicAndPartition, std::pair<uint16_t, std::vector<uint64_t>>>();
            for (const auto kv : request.request_info()) {
              const auto topicAndPartition = kv.first;
              const auto time = kv.second.first;
              const auto maxNumOffset = kv.second.second;
              const auto offsets = components_->controller.FetchOffsets(topicAndPartition, time, maxNumOffset);
              map[topicAndPartition] = std::make_pair(0, offsets);
            }
            const auto response = command::OffsetResponse(request.correlation_id(), map);
            const auto bytes = std::make_shared<ByteBuffer>(response.to_bytes());
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(bytes->pointer(0), bytes->limit()),
                [this, self, bytes](boost::system::error_code wec, std::size_t) {
                  if (wec) throw std::runtime_error("Can't send a offset response.");
                  ReadSize();
                }
            );
            break;
          }
          case 3 : {
            const auto request = command::TopicMetadataRequest(*buffer.get());
            std::unordered_set<std::string> topics;
            for (const auto &topic : request.topics()) {
              topics.insert(topic);
            }
            const auto metadata = components_->controller.GetTopicMetadata(topics);
            const auto response = command::TopicMetadataResponse {
                components_->controller.GetAliveBrokers(),
                metadata,
                request.correlation_id()
            };
            const auto bytes = std::make_shared<ByteBuffer>(response.to_bytes());
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(bytes->pointer(0), bytes->limit()),
                [this, self, bytes](boost::system::error_code wec, std::size_t) {
                  if (wec) throw std::runtime_error("Can't send a topic metadata response.");
                  cafca::util::debug("TopicMetadataResponse is done.");
                  ReadSize();
                }
            );
            break;
          }
          default: {
            cafca::util::debug("Unknown request.");
            break;
          }
        }
      }
  );
}

}
}
}
