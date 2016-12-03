#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include <apex_types.h>
#include <apex_partition.h>

#include "../circular_buffer.h"

/*
 * This macro simplifies the generation of message buffers.
 * It sets the maximum message size, maximum message number
 * and the buffer to hold them.
 */
#define MESSAGE_BUFFER(nb_message, message_size) \
	.MAX_MESSAGE_SIZE = message_size, \
	.MAX_NB_MESSAGE = nb_message, \
	.buffer = (uint8_t [(message_size * nb_message) + \
		(nb_message * sizeof(size_t)) + 1]) {0}, \
	.circ_buf = {0, 0, 0, (message_size * nb_message) + \
		(nb_message * sizeof(size_t)) + 1}

#define MAX_PROCESSES_PER_PARTITIONS 3

typedef enum {
	LEVEL_A,
	LEVEL_B,
	LEVEL_C,
	LEVEL_D,
	LEVEL_E,
} criticality_t;

typedef enum {
	SAMPLING_PORT,
	QUEUING_PORT,
} PORT_TYPE;

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

struct queuing_port {
	MESSAGE_RANGE_TYPE        MAX_NB_MESSAGE;
	MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE;
	MESSAGE_RANGE_TYPE        NB_MESSAGE;
	QUEUING_DISCIPLINE_TYPE   QUEUING_DISCIPLINE;
	circBuf_t                 circ_buf;
	uint8_t                   *buffer;
};

struct sampling_port {
	MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE;
	SYSTEM_TIME_TYPE          REFRESH_PERIOD;
	circBuf_t                 circ_buf;
	uint8_t                   *buffer;
};

typedef struct {
	bool                      is_queuing_port;
	union {
		struct queuing_port   q_buf;
		struct sampling_port  s_buf;
	};
	bool                      activated;
	void                      *channel_link;
	PORT_DIRECTION_TYPE       PORT_DIRECTION;
	NAME_TYPE                 portname;
} port_t;

typedef struct {
	uint32_t                  stackpointer;
	uint8_t                   exc_return_value;
	uint32_t				  tickStamp;
	PROCESS_STATUS_TYPE; /* unamed struct with -fms-extensions */
} process_t;

typedef struct {
	PARTITION_ID_TYPE         IDENTIFIER;
	NAME_TYPE                 partitionname;
	criticality_t             criticality; /* Not used */
	bool                      systempartion; /* Not used*/
	void                      (*entrypoint)(void);
	APEX_INTEGER              nb_ports;
	port_t                    *ports;
	uint32_t                  nb_processes;
	uint32_t                  index_running_process;
	process_t                 processes[MAX_PROCESSES_PER_PARTITIONS];
} partition_t;

typedef struct{
	int                       channelidentifier;
	NAME_TYPE                 channelname;
	APEX_INTEGER              nb_ports;
	port_t                    **ports;
} channel_t;

typedef struct {
	mem_type_t                type;
	uint32_t                  size;
	mem_access_t              access;
	uint32_t                  address;
} mem_req_t;

typedef struct{
	PARTITION_ID_TYPE         IDENTIFIER;
	NAME_TYPE                 partitionname;
	uint32_t                  arr_size;
	mem_req_t                 *memory_arr;
} part_mem_t;

typedef struct {
    int windowidentifier;
    float windowstartseconds;
    float windowdurationseconds;
    bool partitionperiodstart;
} window_schedule;

typedef struct {
	int partitionidentifier;
    char partitionname[32];
    float peroidseconds;
    float perioddurationseconds;
    const window_schedule *window_arr;
} partition_schedule;

#endif // !TYPES_H