#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#include "arinc/statics.h"

#define MAX_NUM_WINDOWS 20

partition_t* scheduler_partitionScheduler(void);
process_t* scheduler_processScheduler(partition_t *part);
void scheduler_buildSchedule(void);

#endif // !SCHEDULER_H
