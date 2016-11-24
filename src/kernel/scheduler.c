#include <stm32f4xx_hal.h>

#include "scheduler.h"

static uint8_t indexActivePartition = -1;

static uint16_t timings1[MAX_PARTITION] = {1000, 1000, 3000};
static uint16_t timings2 = 0;

void scheduler_partitionScheduler(void)
{
	if(timings2 <= HAL_GetTick()){
		indexActivePartition = (indexActivePartition + 1) % partition_getNumberOfPartitions();
		timings2 = HAL_GetTick() + timings1[indexActivePartition];
	}
}

process_t* scheduler_processScheduler(void)
{
	int i;
	process_t *tmpProcess, *activeProcess;
	
	activeProcess = &configured_partitions[indexActivePartition].processes[0];

	for(i=1; i < MAX_PROCESSES_PER_PARTITIONS; i++)
	{
		tmpProcess = &configured_partitions[indexActivePartition].processes[i];
		if(tmpProcess->stackpointer == 0) break;		
		else
			if(tmpProcess->apexDetails.CURRENT_PRIORITY > activeProcess->apexDetails.CURRENT_PRIORITY)
				activeProcess = tmpProcess;
	}
	return activeProcess;
}