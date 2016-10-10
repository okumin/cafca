#ifndef CAFCA_CONNECTION_H
#define CAFCA_CONNECTION_H

#include <boost/asio.hpp>
#include <cafca/server/cafca_components.h>
namespace cafca {
namespace server {
namespace api {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  Connection(boost::asio::ip::tcp::socket &socket,
             const std::shared_ptr<cafca::server::CafcaComponents> components);

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Connection(Connection&&) = delete;
  Connection& operator=(Connection&&) = delete;

  void Start();
  
 private:
  boost::asio::ip::tcp::socket socket_;
  std::shared_ptr<cafca::server::CafcaComponents> components_;

  void ReadSize();

  void ReadPayload(const uint32_t size);
};

}
}
}

#endif //CAFCA_CONNECTION_H
