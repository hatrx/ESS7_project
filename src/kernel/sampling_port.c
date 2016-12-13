#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <apex_sampling.h>

#include "ports.h"
#include "../xml_data.h"
#include "kernel/part_scheduler.h"


void init_sampling_ports(void)
{
    const size_t nb_channels = sizeof(connection_table) / sizeof(channel_t);
    for (size_t i = 0; i < nb_channels; i++) {
        channel_t *channel = &connection_table[i];
        port_t **port_list = connection_table[i].ports;

        for (APEX_INTEGER n = 0; n < channel->nb_ports; ++n) {
            port_list[n]->channel_link = channel;
        }
    }
}


void create_sampling_port(
    /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
    /*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE,
    /*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION,
    /*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD,
    /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
    /*out*/ RETURN_CODE_TYPE           *RETURN_CODE)
{
    partition_t *this_partition = getActivePartition();
    port_t *ports = this_partition->ports;

    for (APEX_INTEGER n = 0; n < this_partition->nb_ports; ++n) {
        if (!strcmp(ports[n].portname, SAMPLING_PORT_NAME) &&
        ports[n].s_buf.MAX_MESSAGE_SIZE == MAX_MESSAGE_SIZE &&
        ports[n].s_buf.REFRESH_PERIOD == REFRESH_PERIOD &&
        ports[n].s_buf.PORT_DIRECTION == PORT_DIRECTION)
        {
            ports[n].activated = true;
            *SAMPLING_PORT_ID = n;
            *RETURN_CODE = NO_ERROR;
            return;
        }
    }

    *RETURN_CODE = INVALID_PARAM;
}


void write_sampling_message(
    /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
    /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,    /* by reference */
    /*in */ MESSAGE_SIZE_TYPE          LENGTH,
    /*out*/ RETURN_CODE_TYPE           *RETURN_CODE)
{
    partition_t *this_partition = getActivePartition();

    /* Return error if invalid SAMPLING_PORT_ID */
    if (SAMPLING_PORT_ID >= this_partition->nb_ports) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }

    port_t *port = &this_partition->ports[SAMPLING_PORT_ID];
    channel_t *chan = port->channel_link;

    /*	If the message is never recieved by at least one,
    'message_sent' will stay false. */
    bool message_sent = false;

    port_t **ports = chan->ports;
    for (APEX_INTEGER i = 0; i < chan->nb_ports; i++) {
        port_t *port = ports[i];

        if (port->is_queuing_port ||
            !port->activated ||
            !(port->s_buf.PORT_DIRECTION == DESTINATION))
        {
            /* Not a valid destination */
            continue;
        }

        if (LENGTH > port->s_buf.MAX_MESSAGE_SIZE) {
            /* Message too long. */
            continue;
        }

        port->s_buf.mess_size = LENGTH;
        for (int32_t i = 0; i < LENGTH; ++i) {
            port->s_buf.buffer[i] = ((uint8_t *)MESSAGE_ADDR)[i];
        }

        message_sent = true;
    }

    if (message_sent) {
        *RETURN_CODE = NO_ERROR;
    } else {
        /* No destination ports */
        *RETURN_CODE = NO_ACTION;
    }
}


void read_sampling_message(
    /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
    /*out*/ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
    /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
    /*out*/ VALIDITY_TYPE              *VALIDITY,
    /*out*/ RETURN_CODE_TYPE           *RETURN_CODE)
{
    (void)VALIDITY;
    partition_t *this_partition = getActivePartition();

    /* Return error if invalid SAMPLING_PORT_ID */
    if (SAMPLING_PORT_ID >= this_partition->nb_ports) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }

    port_t *port = &this_partition->ports[SAMPLING_PORT_ID];
    *LENGTH = port->s_buf.mess_size;
    for (int32_t i = 0; i < *LENGTH; i++) {
        MESSAGE_ADDR[i] = port->s_buf.buffer[i];
    }

    *RETURN_CODE = NO_ERROR;
}


void get_sampling_port_id(
    /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
    /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
    /*out*/ RETURN_CODE_TYPE           *RETURN_CODE)
{
    partition_t *this_partition = getActivePartition();
    port_t *ports = this_partition->ports;

    for (APEX_INTEGER n = 0; n < this_partition->nb_ports; ++n) {
        if (!strcmp(ports[n].portname, SAMPLING_PORT_NAME)) {
            *SAMPLING_PORT_ID = n;
            *RETURN_CODE = NO_ERROR;
            return;
        }
    }

    *RETURN_CODE = INVALID_PARAM;
}


void get_sampling_port_status(
    /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
    /*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS,
    /*out*/ RETURN_CODE_TYPE           *RETURN_CODE)
{
    partition_t *this_partition = getActivePartition();

    /* Return error if invalid SAMPLING_PORT_ID */
    if (SAMPLING_PORT_ID >= this_partition->nb_ports) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }

    port_t port = this_partition->ports[SAMPLING_PORT_ID];

    SAMPLING_PORT_STATUS->LAST_MSG_VALIDITY = port.s_buf.LAST_MSG_VALIDITY;
    SAMPLING_PORT_STATUS->REFRESH_PERIOD = port.s_buf.REFRESH_PERIOD;
    SAMPLING_PORT_STATUS->MAX_MESSAGE_SIZE = port.s_buf.MAX_MESSAGE_SIZE;
    SAMPLING_PORT_STATUS->PORT_DIRECTION = port.s_buf.PORT_DIRECTION;

    /* A procedure is needed for SAMPLING_PORT_STATUS->WAITING_PROCESSES */

    *RETURN_CODE = NO_ERROR;
}
