#ifndef QUEUING_H
#define QUEUING_H

#include <stdint.h>

#include <apex_queuing.h>


extern uint8_t curr_partition_id;

void init_queuing_ports(void);

void create_queuing_port(
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
	/*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
	/*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
	/*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

void recieve_queuing_message(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
	/*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE);


#endif /* QUEUING_H */
