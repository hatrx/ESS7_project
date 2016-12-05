#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>
#include <stdbool.h>

#include <apex_queuing.h>
#include <apex_sampling.h>

#include "../circular_buffer.h"


struct queuing_port {
	QUEUING_PORT_STATUS_TYPE;
	circBuf_t                 circ_buf;
	uint8_t                   *buffer;
};

struct sampling_port {
	SAMPLING_PORT_STATUS_TYPE;
	uint32_t                  mess_size;
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
	NAME_TYPE                 portname;
} port_t;

typedef struct{
	uint8_t                   id;
	NAME_TYPE                 channelname;
	APEX_INTEGER              nb_ports;
	port_t                    **ports;
} channel_t;


void init_queuing_ports(void);
void init_sampling_ports(void);

void create_queuing_port(
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
	/*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
	/*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
	/*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void send_queuing_message(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
	/*in */ MESSAGE_SIZE_TYPE        LENGTH,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void recieve_queuing_message(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
	/*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void get_queuing_port_id(
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void get_queuing_port_status(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void create_sampling_port(
	/*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
	/*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE,
	/*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION,
	/*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD,
	/*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE           *RETURN_CODE);

void write_sampling_message(
	/*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
	/*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,    /* by reference */
	/*in */ MESSAGE_SIZE_TYPE          LENGTH,
	/*out*/ RETURN_CODE_TYPE           *RETURN_CODE);

void read_sampling_message(
	/*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
	/*out*/ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
	/*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
	/*out*/ VALIDITY_TYPE              *VALIDITY,
	/*out*/ RETURN_CODE_TYPE           *RETURN_CODE);

void get_sampling_port_id(
	/*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
	/*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE           *RETURN_CODE);

void get_sampling_port_status(
	/*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
	/*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS,
	/*out*/ RETURN_CODE_TYPE           *RETURN_CODE);


#endif /* PORTS_H */
