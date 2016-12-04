#include <stdio.h>
#include <stddef.h>
#include "partition.h"
#include "statics.h"
#include "process.h"
#include <kernel/error_handler.h>


mem_req_t get_ram_info(partition_t *partition)
{
	PARTITION_ID_TYPE id = partition->IDENTIFIER;

	const uint32_t nb_mem_infos = sizeof(partition_memory) / sizeof(part_mem_t);
	for (size_t i = 0; i < nb_mem_infos; ++i) {
		if (id != partition_memory[i].IDENTIFIER) {
			continue;
		}

		const uint32_t nb_mem = partition_memory[i].arr_size;
		for (size_t n = 0; n < nb_mem; n++) {
			if (partition_memory[i].memory_arr[n].type == DATA) {
				return partition_memory[i].memory_arr[n];
			}
		}
	}

	/* KERNEL PANIC */
	Error_Handler();
}


void init_partitions(void) {
	// Set up idle partition
	partition_t *part = &partitions[0];
	process_t* processes = part->processes;

	PROCESS_ATTRIBUTE_TYPE attributes = {
			.PERIOD = 0,
			.TIME_CAPACITY = 0,
			.ENTRY_POINT = part->entrypoint,
			.BASE_PRIORITY = 1,
			.DEADLINE = SOFT,
			.NAME = "main",
		};

	PROCESS_ID_TYPE processId;
	RETURN_CODE_TYPE RETURN_CODE;
	create_process(part, 0x20000100, &attributes, &processId, &RETURN_CODE);

	if (RETURN_CODE != NO_ERROR) {
		/* KERNEL PANIC */
		Error_Handler();
	}

	if (processId != 0) {
		/* KERNEL PANIC */
		Error_Handler();
	}

	// Set up the rest of the partitions
	const uint32_t nb_partitions = sizeof(partitions) / sizeof(partition_t);
	for (size_t i = 1; i < nb_partitions; i++) {
		part = &partitions[i];
		processes = part->processes;

		part->nb_processes = 1;
		part->index_running_process = 0;

		const mem_req_t mem_req = get_ram_info(part);

		PROCESS_ATTRIBUTE_TYPE attributes = {
			.PERIOD = 0,
			.TIME_CAPACITY = 0,
			.ENTRY_POINT = part->entrypoint,
			.BASE_PRIORITY = 1,
			.DEADLINE = SOFT,
			.NAME = "main",
		};

		create_process(part, mem_req.address, &attributes, &processId, &RETURN_CODE);

		if (RETURN_CODE != NO_ERROR) {
			/* KERNEL PANIC */
			Error_Handler();
		}

		if (processId != 0) {
			/* KERNEL PANIC */
			Error_Handler();
		}

		/* Set the rest og the processes as DORMANT */
		for (size_t n = 1; n < MAX_PROCESSES_PER_PARTITIONS; ++n) {
			processes[n].PROCESS_STATE = DORMANT;
		}
	}
}
