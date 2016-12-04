#ifndef WINDOW_H

#include "partition.h"

typedef struct {
    partition_t*        partition;
    uint32_t            frameStartTimeMillis;
    uint32_t            durationMillis;
} window_t;

#endif // !WINDOW_H