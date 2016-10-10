#ifndef CAFCA_PARTITION_MEMBERS_H
#define CAFCA_PARTITION_MEMBERS_H

#include <unordered_set>
#include "active_members.h"
namespace cafca {
namespace server {
namespace cluster {

struct PartitionMembers {
  const ActiveMembers active_members;
  const uint32_t controller_epoch;
  const std::unordered_set<uint32_t> all_replicas;
};

}
}
}

#endif //CAFCA_PARTITION_MEMBERS_H
