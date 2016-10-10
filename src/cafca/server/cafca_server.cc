#include "cafca_server.h"

namespace cafca {
namespace server {

CafcaServer::CafcaServer(boost::asio::io_service &service)
    : service_(service),
      components_(std::make_shared<CafcaComponents>()),
      socket_server_(service, components_) {}

}
}
