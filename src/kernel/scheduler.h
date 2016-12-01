#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#include "arinc/partition.h"

void scheduler_partitionScheduler(void);
process_t* scheduler_processScheduler(partition_t *part);

#endif // !SCHEDULER_H
