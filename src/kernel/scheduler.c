#include <stm32f4xx_hal.h>

#include "scheduler.h"
#include "arinc/statics.h"
#include "arinc/queuing_port.h"

static uint8_t indexActivePartition = -1;

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

process_t* scheduler_processScheduler(void)
{
	process_t *tmpProcess, *activeProcess;
	partition_t *activePartition;

	activePartition = &partitions[indexActivePartition];
	activeProcess = &activePartition->processes[0];

	for (int i = 1; i < MAX_PROCESSES_PER_PARTITIONS; ++i)
	{
		tmpProcess = &partitions[indexActivePartition].processes[i];
		if (tmpProcess->stackpointer == 0)
		{
		    break;
		}
		else if (tmpProcess->apexDetails.PROCESS_STATE == READY)
		{
		    if (tmpProcess->apexDetails.CURRENT_PRIORITY > activeProcess->apexDetails.CURRENT_PRIORITY)
		    {
				if (tmpProcess->tickStamp < activeProcess->tickStamp)
				{
					activeProcess = tmpProcess;
				}
		    }
		}
	}

	return activeProcess;
}
