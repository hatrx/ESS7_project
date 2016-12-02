#include <stdint.h>

#include "../circular_buffer.h"
#include "statics.h"
#include "queuing_port.h"


port_t p_dummy_1[1] = {{
	.is_queuing_port = true,
	.q_buf = {
		MESSAGE_BUFFER(32, 32),
	},
	.PORT_DIRECTION = SOURCE,
	.portname = "print_1",
},};

port_t p_dummy_2[1] = {{
	.is_queuing_port = true,
	.q_buf = {
		MESSAGE_BUFFER(32, 32),
	},
	.PORT_DIRECTION = SOURCE,
	.portname = "print_2",
},};

port_t p_stio_sys[1] = {{
	.is_queuing_port = true,
	.q_buf = {
		MESSAGE_BUFFER(32, 32),
	},
	.PORT_DIRECTION = DESTINATION,
	.portname = "sys_stio",
},};

partition_t partitions[3] = {{
	.IDENTIFIER = 1,
	.partitionname = "dummy_1",
	.criticality = LEVEL_A,
	.systempartion = false,
	.entrypoint = &dummy1_main,
	.nb_ports = 1,
	.ports = p_dummy_1,
},{
	.IDENTIFIER = 2,
	.partitionname = "dummy_2",
	.criticality = LEVEL_A,
	.systempartion = false,
	.entrypoint = &dummy2_main,
	.nb_ports = 1,
	.ports = p_dummy_2,
},{
	.IDENTIFIER = 3,
	.partitionname = "stio_sys",
	.criticality = LEVEL_A,
	.systempartion = true,
	.entrypoint = &stdio_sys_main,
	.nb_ports = 1,
	.ports = p_stio_sys,
},};

mem_req_t mem_dummy_1[2] = {{
	.type = CODE,
	.size = 8000,
	.access = READ_ONLY,
	.address = (void *)0x8010000,
},{
	.type = DATA,
	.size = 8000,
	.access = READ_WRITE,
	.address = (void *)0x20010000,
},};

mem_req_t mem_dummy_2[2] = {{
	.type = CODE,
	.size = 8000,
	.access = READ_ONLY,
	.address = (void *)0x8012000,
},{
	.type = DATA,
	.size = 8000,
	.access = READ_WRITE,
	.address = (void *)0x20012000,
},};

mem_req_t mem_stdio_sys[2] = {{
	.type = CODE,
	.size = 8000,
	.access = READ_ONLY,
	.address = (void *)0x8014000,
},{
	.type = DATA,
	.size = 8000,
	.access = READ_WRITE,
	.address = (void *)0x20014000,
},};

part_mem_t partition_memmory[3] = {{
	.IDENTIFIER = 1,
	.partitionname = "dummy_1",
	.memory_arr = mem_dummy_1,
},{
	.IDENTIFIER = 2,
	.partitionname = "dummy_2",
	.memory_arr = mem_dummy_2,
},{
	.IDENTIFIER = 3,
	.partitionname = "stio_sys",
	.memory_arr = mem_stdio_sys,
},};

port_t *stio_channel_ports[3] = {
	&p_dummy_1[0],
	&p_dummy_2[0],
	&p_stio_sys[0],
};

channel_t connection_table[1] = {{
		.channelidentifier = 1,
		.channelname = "stio_channel",
		.nb_ports = 3,
		.ports = stio_channel_ports,
	},
};
