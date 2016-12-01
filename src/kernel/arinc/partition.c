#include <stdio.h>
#include <stddef.h>
#include "partition.h"
#include "statics.h"


void init_partitions(void) {
	const uint32_t nb_processes = sizeof(partitions) / sizeof(partition_t);
	for (size_t i = 0; i < nb_processes; i++) {
		partitions[i].nb_processes = 0;
		partitions[i].index_running_process = 0;
	}
}

void init_partition(partition_t* partition_)
{
	// Change the state of the first process to READY. The first process is
	// always the main process.
	partition_->processes[0].PROCESS_STATE = READY;
}
