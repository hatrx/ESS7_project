#ifndef STATICS_H
#define STATICS_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "APEX/apex_queuing.h"
#include "APEX/apex_sampling.h"
#include "APEX/apex_partition.h"

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


typedef enum {
	LEVEL_A,
	LEVEL_B,
	LEVEL_C,
	LEVEL_D,
	LEVEL_E,
} CRITICALITY;

typedef enum {
	SAMPLING_PORT,
	QUEUING_PORT,
} PORT_TYPE;

struct queuing_port {
	MESSAGE_RANGE_TYPE        MAX_NB_MESSAGE;
	MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE;
	MESSAGE_RANGE_TYPE        NB_MESSAGE;
	QUEUING_DISCIPLINE_TYPE   QUEUING_DISCIPLINE; /* Not used*/
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
	PARTITION_ID_TYPE         IDENTIFIER;
	NAME_TYPE                 partitionname;
	CRITICALITY               criticality; /* Not used */
	bool                      systempartion; /* Not used*/
	void                      (*entrypoint)(void);
	APEX_INTEGER              nb_ports;
	port_t                    *ports;
} partition_t;

typedef struct{
	int                       channelidentifier;
	NAME_TYPE                 channelname;
	APEX_INTEGER              nb_ports;
	port_t                    **ports;
} channel_t;


void dummy1_main(void);
void dummy2_main(void);
void stdio_sys_main(void);


extern partition_t partitions[3];
extern channel_t connection_table[1];


#endif /* STATICS_H */
