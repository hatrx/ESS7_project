#ifndef PART_SCHEDULER_H
#define PART_SCHEDULER_H

#include <stdint.h>

#include "../xml_data.h"

#define MAX_NUM_WINDOWS 20

partition_t* scheduler_partitionScheduler(void);
process_t* scheduler_processScheduler(partition_t *part);
partition_t* getActivePartition(void);
void process_stop_self(void);
void scheduler_buildSchedule(void);

#endif /* PART_SCHEDULER_H */
