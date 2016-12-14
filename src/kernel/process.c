#include <stm32f4xx_hal.h>

#include "process.h"
#include "kernel/context.h"
#include "kernel/part_scheduler.h"
#include "types.h"


static size_t find_dormant_process(process_t *processes, size_t nb_proc)
{
    for (size_t i = 0; i < nb_proc; ++i) {
        if(processes[i].PROCESS_STATE == DORMANT) {
            return i;
        }
    }

    return -1;
}


void create_process(partition_t *partition, uint32_t memoryAddress, PROCESS_ATTRIBUTE_TYPE* attributes, PROCESS_ID_TYPE* processId, RETURN_CODE_TYPE *RETURN_CODE)
{
    /* TODO: memoryAddress should be calculated and not given */
    const int i = find_dormant_process(partition->processes, MAX_PROCESSES_PER_PARTITIONS);

    if (i == -1) {
        *RETURN_CODE = NOT_AVAILABLE;
        return;
    }
    *RETURN_CODE = NO_ERROR;

    *processId = i;
    process_t *process = &partition->processes[i];

    process->ATTRIBUTES  = *attributes;
    process->CURRENT_PRIORITY = attributes->BASE_PRIORITY;
    attributes->STACK_SIZE = 1024;
    /* Should it maybe be "-" sizeof(ARM_context_state) on the following line? */
    process->stackpointer = memoryAddress + sizeof(ARM_context_state);
    process->exc_return_value = 0xFD;
    process->tickStamp = HAL_GetTick();

    context_setup(attributes->ENTRY_POINT, (void *) process->stackpointer);

    process->PROCESS_STATE = READY;
}


void runtime_create_process(PROCESS_ATTRIBUTE_TYPE *attributes, PROCESS_ID_TYPE *processId, RETURN_CODE_TYPE *RETURN_CODE)
{
    partition_t *partition = getActivePartition();
    int32_t id = partition->id;

    /* Find overall memmory structure for this partition. */
    part_mem_t *p_mem = {0};
    const uint32_t nb_mems = sizeof(partition_memory) / sizeof(part_mem_t);
    for (size_t i = 0; i < nb_mems; i++) {
        if (partition_memory[i].id == id) {
            p_mem = &partition_memory[i];
        }
    }


    /* If not found data is missing, return error. */
    if (p_mem->id == 0) {
        *RETURN_CODE = INVALID_CONFIG;
        return;
    }

    /* Get the DATA memory requirements for this partition. */
    mem_req_t *data_mem = {0};
    for (size_t i = 0; i < p_mem->arr_size; i++) {
        if (p_mem->memory_arr[i].type == DATA) {
            data_mem = &p_mem->memory_arr[i];
        }
    }

    /* Data is invalid or missing, return error. */
    if (data_mem->size == 0) {
        *RETURN_CODE = INVALID_CONFIG;
        return;
    }

    const uint32_t mem_size = attributes->STACK_SIZE;
    /* Not enough space, return error. */
    if ((mem_size + p_mem->mem_offset) > data_mem->size) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }


    p_mem->mem_offset += mem_size;
    const uint32_t addr = data_mem->address + p_mem->mem_offset;

    create_process(partition, addr, attributes, processId, RETURN_CODE);
}
