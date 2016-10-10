#ifndef CAFCA_ACTIVE_MEMBERS_H
#define CAFCA_ACTIVE_MEMBERS_H

#include <vector>

namespace cafca {
namespace server {
namespace cluster {

struct ActiveMembers {
  const uint32_t leader;
  const uint32_t leader_epoch;
  const std::vector<uint32_t> isr;
};

}
}
}

namespace cafca {
namespace util {


}
}


#endif //CAFCA_ACTIVE_MEMBERS_H
