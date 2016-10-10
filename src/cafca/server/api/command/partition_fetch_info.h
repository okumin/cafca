#ifndef CAFCA_PARTITION_FETCH_INFO_H
#define CAFCA_PARTITION_FETCH_INFO_H

#include <stdint.h>
namespace cafca {
namespace server {
namespace api {
namespace command {

struct PartitionFetchInfo {
  const uint64_t offset;
  const uint32_t fetch_size;
};

}
}
}
}

#endif //CAFCA_PARTITION_FETCH_INFO_H
