#ifndef PARTITION_H
#define PARTITION_H


#include <stdint.h>
#include <apex_process.h>

#include "statics.h"


uint8_t process_createProcess(partition_t* , uint32_t , PROCESS_ATTRIBUTE_TYPE* , PROCESS_ID_TYPE* );
void init_partitions(void);
void init_partition(partition_t* );


#endif // PARTITION_H
