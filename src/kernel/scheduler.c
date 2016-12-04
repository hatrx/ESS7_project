#include <stm32f4xx_hal.h>

#include "scheduler.h"
#include "arinc/statics.h"
#include "arinc/queuing_port.h"

uint8_t indexActivePartition = 0;

static uint16_t timings1[4] = {1000, 1000, 1000, 3000};
static uint32_t timings2 = 0;

void scheduler_partitionScheduler(void)
{
	const uint32_t nb_partitions = sizeof(partitions) / sizeof(partition_t);
	if(timings2 <= HAL_GetTick()){
		indexActivePartition = (indexActivePartition + 1) % nb_partitions;
		timings2 = HAL_GetTick() + timings1[indexActivePartition];

		curr_partition_id = partitions[indexActivePartition].IDENTIFIER;
	}
}

process_t* scheduler_processScheduler(partition_t *part)
{
	/* Set state of currently running process to READY. */
	//process_t *curr_process = &part->processes[part->index_running_process];
//	if (curr_process->PROCESS_STATE == RUNNING) {
//		curr_process->PROCESS_STATE = READY;
//	}

	/* Get list of processes in partition. */
	process_t *processes = part->processes;

	/* Find a READY partition as default candidate for execution. */
	process_t *priority_process;
	for (size_t i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++) {
		//printf("%d - %d - %d\n", indexActivePartition, i, processes[i].PROCESS_STATE);
		if (processes[i].PROCESS_STATE == READY) {
			priority_process = &processes[i];
			break;
		}
	}

	/* Attempt to find a better candidate by comparing to default. */
	for (size_t i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++) {
		if (processes[i].PROCESS_STATE != READY)
			continue;

		if (processes[i].CURRENT_PRIORITY < priority_process->CURRENT_PRIORITY)
			continue;

		if (processes[i].tickStamp < priority_process->tickStamp) {
			priority_process = &processes[i];
			part->index_running_process = i;
		}
	}

	/* Set state of process to RUNNING. */
	//priority_process->PROCESS_STATE = RUNNING;

	/* Return pointer to highest priority candidate process */
	return priority_process;
}


void process_stop_self(void)
{
	partition_t *partition = &partitions[indexActivePartition];
	process_t *proc = &partition->processes[partition->index_running_process];

	proc->PROCESS_STATE = DORMANT;
}
