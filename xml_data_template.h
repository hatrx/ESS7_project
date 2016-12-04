#ifndef XML_DATA_H
#define XML_DATA_H

#include "kernel/arinc/types.h" 
#include <apex_queuing.h>
#include <apex_sampling.h>
#include <apex_partition.h>
#define MAX_PROCESSES_PER_PARTITIONS 3
} mem_type_t;

typedef enum {
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE,
} mem_access_t;
	uint32_t                  stackpointer;
	uint8_t                   exc_return_value;
	uint32_t				  tickStamp;
	PROCESS_STATUS_TYPE; /* unamed struct with -fms-extensions */
} process_t;

typedef struct {
	uint32_t                  nb_processes;
	uint32_t                  index_running_process;
	process_t                 processes[MAX_PROCESSES_PER_PARTITIONS];
typedef struct{
	PARTITION_ID_TYPE         IDENTIFIER;
	NAME_TYPE                 partitionname;
	uint32_t                  arr_size;
	mem_req_t                 *memory_arr;

	/* This value could be calculated every time we make a new process,
	   but this is just way easier... */
	uint32_t                  mem_offset;
} part_mem_t;

void idle_main(void);

typedef struct{
	int partitionidentifier;
	char partitionname[32];
	float peroidseconds;
	float perioddurationseconds;
	const window_schedule *window_arr;
} partition_schedule;