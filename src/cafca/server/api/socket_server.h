#ifndef CAFCA_SOCKET_SERVER_H
#define CAFCA_SOCKET_SERVER_H

#include <boost/asio.hpp>
#include <cafca/server/cafca_components.h>
namespace cafca {
namespace server {
namespace api {

class SocketServer {
 public:
  SocketServer(boost::asio::io_service &service,
               const std::shared_ptr<cafca::server::CafcaComponents> components);

  SocketServer(const SocketServer&) = delete;
  SocketServer& operator=(const SocketServer&) = delete;
  SocketServer(SocketServer&&) = delete;
  SocketServer& operator=(SocketServer&&) = delete;

 private:
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  const std::shared_ptr<cafca::server::CafcaComponents> components_;

  void Accept();
};

}
}
}

#endif //CAFCA_SOCKET_SERVER_H
