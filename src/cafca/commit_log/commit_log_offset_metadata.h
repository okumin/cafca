#ifndef CAFCA_COMMIT_LOG_OFFSET_METADATA_H
#define CAFCA_COMMIT_LOG_OFFSET_METADATA_H

#include <stdint.h>
namespace cafca {
namespace commit_log {

struct CommitLogOffsetMetadata {
  uint64_t message_offset;
  uint64_t segment_base_offset;
  uint32_t relative_position_in_segment;
};

}
}

#endif //CAFCA_COMMIT_LOG_OFFSET_METADATA_H
