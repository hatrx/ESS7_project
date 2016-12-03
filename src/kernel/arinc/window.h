#ifndef WINDOW_H

#include "partition.h"

typedef struct {
    partition_t*        partition;
    float               frameStartTime;
    float               duration;
} window_t;

#endif // !WINDOW_H