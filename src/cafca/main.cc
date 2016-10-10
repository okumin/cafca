#include <boost/asio.hpp>
#include <cafca/server/cafca_server.h>
#include <cafca/util/logging.h>

int main(int argc, char* argv[]) {

  cafca::util::debug("Start");

  try {
    boost::asio::io_service service;
    const cafca::server::CafcaServer server(service);
    service.run();
  } catch (std::exception &e) {
    cafca::util::debug("Exception: ", e.what());
  }

  cafca::util::debug("Finish");

  return 0;
}
