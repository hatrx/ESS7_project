#include <stdio.h>
#include "partition.h"
#include "kernel/context.h"

static process_t* processes[MAX_PROCESSES_PER_PARTITIONS*MAX_PARTITION];

uint8_t process_createProcess(partition_t* partition_, uint32_t memoryAddress, PROCESS_ATTRIBUTE_TYPE* attributes, PROCESS_ID_TYPE* processId)
{
    uint8_t i;
    process_t* process;
    PROCESS_STATUS_TYPE* processStatus;
    static PROCESS_ID_TYPE pid = 0;

    for (i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++)
    {
        if(partition_->processes[i].stackpointer == 0) break;
    }
    if(i == MAX_PROCESSES_PER_PARTITIONS) return INVALID_CONFIG;

    process = &partition_->processes[i];
    processStatus = &process->apexDetails;
    processStatus->ATTRIBUTES       = *attributes;
    processStatus->CURRENT_PRIORITY = attributes->BASE_PRIORITY;
    processStatus->PROCESS_STATE    = DORMANT;

    process->stackpointer = memoryAddress + sizeof(ARM_context_state);

    processes[pid] = process;

    context_setup(attributes->ENTRY_POINT, (void *) process->stackpointer);

    *processId = pid++;
    return NO_ERROR;
}

process_t* process_getByProcessId(PROCESS_ID_TYPE processId)
{
    return processes[processId];
}