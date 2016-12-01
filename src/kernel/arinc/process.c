#include <stdio.h>
#include <stm32f4xx_hal.h>

#include "process.h"
#include "kernel/context.h"


void init_processes(void) {
	const uint32_t nb_partitions = sizeof(partitions) / sizeof(partition_t);

	for (size_t i = 0; i < nb_partitions; i++) {
		process_t* processes = partitions[i].processes;

		for (size_t n = 0; n < MAX_PROCESSES_PER_PARTITIONS; ++n) {
			processes[n].PROCESS_STATE = DORMANT;
		}
	}
}


static int find_dormant_process(partition_t *partition)
{
	for (int i = 0; i < MAX_PROCESSES_PER_PARTITIONS; ++i) {
		if(partition->processes[i].PROCESS_STATE == DORMANT) {
			return i;
		}
	}

	return -1;
}


void create_process(partition_t* partition, uint32_t memoryAddress, PROCESS_ATTRIBUTE_TYPE* attributes, PROCESS_ID_TYPE* processId, RETURN_CODE_TYPE *RETURN_CODE)
{
	/* TODO: memoryAddress should be calculated and not given */
	const int i = find_dormant_process(partition);

	if (i == -1) {
		*RETURN_CODE = NOT_AVAILABLE;
		return;
	}
	*RETURN_CODE = NO_ERROR;

	*processId = i;
	process_t *process = &partition->processes[i];

	process->ATTRIBUTES  = *attributes;
	process->CURRENT_PRIORITY = attributes->BASE_PRIORITY;
	process->stackpointer = memoryAddress + sizeof(ARM_context_state);
	process->exc_return_value = 0xFD;
	process->tickStamp = HAL_GetTick();

	context_setup(attributes->ENTRY_POINT, (void *) process->stackpointer);

	process->PROCESS_STATE = READY;
}
