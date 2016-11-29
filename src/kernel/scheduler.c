#include <stm32f4xx_hal.h>

#include "scheduler.h"
#include "statics.h"

static uint8_t indexActivePartition = -1;

static uint16_t timings1[4] = {1000, 1000, 1000, 3000};
static uint32_t timings2 = 0;

void scheduler_partitionScheduler(void)
{
	const uint32_t nb_partitions = sizeof(test_partitions) / sizeof(partition_t);
	if(timings2 <= HAL_GetTick()){
		indexActivePartition = (indexActivePartition + 1) % nb_partitions;
		timings2 = HAL_GetTick() + timings1[indexActivePartition];
	}
}

process_t* scheduler_processScheduler(void)
{
	process_t *tmpProcess, *activeProcess;
	partition_t *activePartition;

	activePartition = &test_partitions[indexActivePartition];
	activeProcess = &activePartition->processes[0];

	for (int i = 1; i < MAX_PROCESSES_PER_PARTITIONS; ++i)
	{
		tmpProcess = &test_partitions[indexActivePartition].processes[i];
		if(tmpProcess->stackpointer == 0) {
			break;
		} else if (tmpProcess->apexDetails.CURRENT_PRIORITY > activeProcess->apexDetails.CURRENT_PRIORITY) {
			activeProcess = tmpProcess;
		}
	}

	return activeProcess;
}
