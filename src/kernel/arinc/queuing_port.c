#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <apex_queuing.h>

#include "ports.h"
#include "../../xml_data.h"
#include "kernel/scheduler.h"


void init_queuing_ports(void)
{
	const size_t nb_partitions =  sizeof(partitions) / sizeof(partition_t);
	for (size_t i = 0; i < nb_partitions; ++i) {
		port_t *ports = (port_t *)partitions[i].ports;

		for (APEX_INTEGER n = 0; n < partitions[i].nb_ports; ++n) {
			ports[n].activated = false;

			if (ports[n].is_queuing_port) {
				ports[n].q_buf.circ_buf.buffer = ports[n].q_buf.buffer;
				ports[n].q_buf.NB_MESSAGE = 0;
			} else {
				/* TODO: Need to setup ports for the sampling ports as well. */
			}
		}
	}

	const size_t nb_channels = sizeof(connection_table) / sizeof(channel_t);
	for (size_t i = 0; i < nb_channels; i++) {
		channel_t *channel = &connection_table[i];
		port_t **port_list = connection_table[i].ports;

		for (APEX_INTEGER n = 0; n < channel->nb_ports; ++n) {
			port_list[n]->channel_link = channel;
		}
	}
}


void create_queuing_port(
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
	/*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
	/*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
	/*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	(void) QUEUING_DISCIPLINE;

	partition_t *this_partition = getActivePartition();
	port_t *ports = this_partition->ports;

	for (APEX_INTEGER n = 0; n < this_partition->nb_ports; ++n) {
		if (!strcmp(ports[n].portname, QUEUING_PORT_NAME) &&
			ports[n].q_buf.MAX_MESSAGE_SIZE == MAX_MESSAGE_SIZE &&
			ports[n].q_buf.MAX_NB_MESSAGE == MAX_NB_MESSAGE &&
			ports[n].q_buf.PORT_DIRECTION == PORT_DIRECTION)
		{
			ports[n].activated = true;
			*QUEUING_PORT_ID = n;
			*RETURN_CODE = NO_ERROR;
			return;
		}
	}

	*RETURN_CODE = INVALID_PARAM;
}


void send_queuing_message(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
	/*in */ MESSAGE_SIZE_TYPE        LENGTH,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	(void)TIME_OUT; /* Unused parameter */

	partition_t *this_partition = getActivePartition();

	/* Return error if invalid QUEUING_PORT_ID */
	if (QUEUING_PORT_ID >= this_partition->nb_ports) {
		*RETURN_CODE = INVALID_PARAM;
		return;
	}

	port_t *port = &this_partition->ports[QUEUING_PORT_ID];
	channel_t *chan = port->channel_link;

	/*	If the message is never recieved by at least one,
	 	'message_sent' will stay false. */
	bool message_sent = false;

	port_t **ports = chan->ports;
	for (APEX_INTEGER i = 0; i < chan->nb_ports; i++) {
		port_t *port = ports[i];

		if (!port->is_queuing_port ||
			!port->activated ||
			!(port->q_buf.PORT_DIRECTION == DESTINATION)) {
			/* Not a valid destination */
			continue;
		}

		if (port->q_buf.NB_MESSAGE >= port->q_buf.MAX_NB_MESSAGE) {
			/* Too many messages in buffer. */
			continue;
		}

		if (LENGTH > port->q_buf.MAX_MESSAGE_SIZE) {
			/* Message too long. */
			continue;
		}

		if (push_item(&port->q_buf.circ_buf, (uint8_t *)MESSAGE_ADDR, LENGTH)) {
			/*	We should get clear on the action to take
				when destination buffer is full. */
			continue;
		}

		++port->q_buf.NB_MESSAGE;
		message_sent = true;
	}

	if (message_sent) {
		*RETURN_CODE = NO_ERROR;
	} else {
		/* No destination ports */
		*RETURN_CODE = NO_ACTION;
	}
}


void recieve_queuing_message(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
	/*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	(void)TIME_OUT; /* Unused parameter */

	partition_t *this_partition = getActivePartition();

	/* Return error if invalid QUEUING_PORT_ID */
	if (QUEUING_PORT_ID >= this_partition->nb_ports) {
		*RETURN_CODE = INVALID_PARAM;
		return;
	}

	port_t *port = &this_partition->ports[QUEUING_PORT_ID];

	if (pop_item(&port->q_buf.circ_buf, MESSAGE_ADDR, (size_t *)LENGTH)) {
		*RETURN_CODE = NO_ACTION;
	} else {
		--port->q_buf.NB_MESSAGE;
		*RETURN_CODE = NO_ERROR;
	}
}


void get_queuing_port_id(
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	partition_t *this_partition = getActivePartition();
	port_t *ports = this_partition->ports;

	for (APEX_INTEGER n = 0; n < this_partition->nb_ports; ++n) {
		if (!strcmp(ports[n].portname, QUEUING_PORT_NAME)) {
			*QUEUING_PORT_ID = n;
			*RETURN_CODE = NO_ERROR;
			return;
		}
	}

	*RETURN_CODE = INVALID_PARAM;
}


void get_queuing_port_status(
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	partition_t *this_partition = getActivePartition();

	/* Return error if invalid QUEUING_PORT_ID */
	if (QUEUING_PORT_ID >= this_partition->nb_ports) {
		*RETURN_CODE = INVALID_PARAM;
		return;
	}

	port_t port = this_partition->ports[QUEUING_PORT_ID];

	QUEUING_PORT_STATUS->NB_MESSAGE = port.q_buf.NB_MESSAGE;
	QUEUING_PORT_STATUS->MAX_NB_MESSAGE = port.q_buf.MAX_NB_MESSAGE;
	QUEUING_PORT_STATUS->MAX_MESSAGE_SIZE = port.q_buf.MAX_MESSAGE_SIZE;
	QUEUING_PORT_STATUS->PORT_DIRECTION = port.q_buf.PORT_DIRECTION;

	/* A procedure is needed for QUEUING_PORT_STATUS->WAITING_PROCESSES */

	*RETURN_CODE = NO_ERROR;
}
