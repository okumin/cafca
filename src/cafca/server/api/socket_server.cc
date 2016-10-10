#include <cafca/util/logging.h>
#include "socket_server.h"
#include "connection.h"

namespace cafca {
namespace server {
namespace api {

using boost::asio::ip::tcp;

SocketServer::SocketServer(
    boost::asio::io_service &service,
    const std::shared_ptr<cafca::server::CafcaComponents> components)
    : acceptor_(tcp::acceptor(service, tcp::endpoint(tcp::v4(), 9092))),
      socket_(tcp::socket(service)),
      components_(components) {
  Accept();
}

void SocketServer::Accept() {
  acceptor_.async_accept(
      socket_,
      [this](boost::system::error_code ec) {
        if (ec) throw std::runtime_error("Failed accepting.");
        cafca::util::debug("Accepted");
        std::make_shared<Connection>(socket_, components_)->Start();
        Accept();
      }
  );
}

}
}
}
