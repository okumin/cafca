#ifndef CAFCA_KAFKA_SERVER_H
#define CAFCA_KAFKA_SERVER_H

#include <cafca/server/api/socket_server.h>
#include "cafca_components.h"
namespace cafca {
namespace server {

class CafcaServer {
 public:
  explicit CafcaServer(boost::asio::io_service &service);

  CafcaServer(const CafcaServer&) = delete;
  CafcaServer& operator=(const CafcaServer&) = delete;
  CafcaServer(CafcaServer&&) = delete;
  CafcaServer& operator=(CafcaServer&&) = delete;

 private:
  boost::asio::io_service &service_;
  std::shared_ptr<CafcaComponents> components_;
  api::SocketServer socket_server_;
};

}
}

#endif //CAFCA_KAFKA_SERVER_H
