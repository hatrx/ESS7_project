#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include <apex_types.h>
#include <apex_partition.h>
#include "ports.h"

#include "circular_buffer.h"

/*
* This macro simplifies the generation of message buffers.
* It sets the maximum message size, maximum message number
* and the buffer to hold them.
*/
#define MESSAGE_BUFFER(nb_message, message_size) \
    .MAX_MESSAGE_SIZE = message_size, \
    .MAX_NB_MESSAGE = nb_message, \
    .buffer = (uint8_t [(message_size * nb_message) + \
    (message_size * sizeof(size_t))]) {0}, \
    .circ_buf = {0, 0, 0, (message_size * nb_message) + \
    (message_size * sizeof(size_t))}

#define MAX_PROCESSES_PER_PARTITIONS 3

typedef enum {
    LEVEL_A,
    LEVEL_B,
    LEVEL_C,
    LEVEL_D,
    LEVEL_E,
} CRITICALITY;

typedef enum {
    CODE,
    DATA,
    INPUT_OUTPUT,
} mem_type_t;

typedef enum {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE,
} mem_access_t;

typedef struct {
    uint32_t                  stackpointer;
    uint8_t                   exc_return_value;
    uint32_t				  tickStamp;
    PROCESS_STATUS_TYPE; /* unamed struct with -fms-extensions */
} process_t;

typedef struct {
    uint8_t                   id;
    NAME_TYPE                 partitionname;
    CRITICALITY	              criticality; /* Not used */
    bool                      systempartion; /* Not used*/
    void                      (*entrypoint)(void);
    APEX_INTEGER              nb_ports;
    port_t                    *ports;
    uint32_t                  nb_processes;
    uint32_t                  index_running_process;
    process_t                 processes[MAX_PROCESSES_PER_PARTITIONS];
} partition_t;

typedef struct {
    mem_type_t                type;
    uint32_t                  size;
    mem_access_t              access;
    uint32_t                  address;
} mem_req_t;

typedef struct{
    uint8_t                   id;
    NAME_TYPE                 partitionname;
    uint32_t                  arr_size;
    mem_req_t                 *memory_arr;

    /* This value could be calculated every time we make a new process,
    but this is just way easier... */
    uint32_t                  mem_offset;
} part_mem_t;

typedef struct {
    uint8_t id;
    uint32_t windowstartmilliseconds;
    uint32_t windowdurationmilliseconds;
    bool partitionperiodstart;
} window_schedule;

typedef struct {
    uint8_t id;
    char partitionname[32];
    float peroidseconds;
    float perioddurationseconds;
    const uint8_t numWindows;
    const window_schedule *window_arr;
} partition_schedule;

#endif // !TYPES_H
