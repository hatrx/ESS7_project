#ifndef PARTITION_H
#define PARTITION_H

#include <stdint.h>
#include <apex_process.h>

#include "hedder.h"

#define MAX_PARTITION	3
#define MAX_PROCESSES_PER_PARTITIONS 3

typedef struct _process {
    uint32_t            stackpointer;
    uint8_t             exc_return_value;
    PROCESS_STATUS_TYPE apexDetails;
} process_t;

typedef struct _partition {
    partition           partitionInfo;
    process_t           processes[MAX_PROCESSES_PER_PARTITIONS];
} partition_t;

extern partition_t configured_partitions[MAX_PARTITION];

uint8_t process_createProcess(partition_t* , uint32_t , PROCESS_ATTRIBUTE_TYPE* , PROCESS_ID_TYPE* );
partition_t* partition_create(partition* );
uint8_t partition_getNumberOfPartitions(void);




#endif // PARTITION_H