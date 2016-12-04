#include <stdint.h>

#include "xml_data.h"
#include "kernel/circular_buffer.h"
#include "kernel/arinc/queuing_port.h"

port_t p_dummy_1[1] = {{
    .is_queuing_port = true,
    .q_buf = { MESSAGE_BUFFER(8, 30), },
    .PORT_DIRECTION = SOURCE,
    .portname = "print_1",
    },};

port_t p_dummy_2[1] = {{
    .is_queuing_port = true,
    .q_buf = { MESSAGE_BUFFER(8, 30), },
    .PORT_DIRECTION = SOURCE,
    .portname = "print_2",
    },};

port_t p_stio_sys[1] = {{
    .is_queuing_port = true,
    .q_buf = { MESSAGE_BUFFER(8, 30), },
    .PORT_DIRECTION = DESTINATION,
    .portname = "sys_stio",
    },};

port_t p_evil[1] = {{
    .is_queuing_port = true,
    .q_buf = { MESSAGE_BUFFER(1, 1), },
    .PORT_DIRECTION = DESTINATION,
    .portname = "temp",
    },};

partition_t partitions[5] = {{ 
    .IDENTIFIER = 0,
    .partitionname = "idle",
    .criticality = LEVEL_E,
    .systempartion = false,
    .entrypoint = &idle_main,
    .nb_ports = 0,
    },{
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
    },{ 
    .IDENTIFIER = 4,
    .partitionname = "evil",
    .criticality = LEVEL_A,
    .systempartion = false,
    .entrypoint = &evil_main,
    .nb_ports = 1,
    .ports = p_evil,
    },};

const window_schedule windowp_dummy_1[2] = {{
    .windowidentifier = 101,
    .windowstartmilliseconds = 0,
    .windowdurationmilliseconds = 20,
    .partitionperiodstart = true,
    },{
    .windowidentifier = 102,
    .windowstartmilliseconds = 100,
    .windowdurationmilliseconds = 20,
    .partitionperiodstart = true,
    },};

const window_schedule windowp_dummy_2[2] = {{
    .windowidentifier = 201,
    .windowstartmilliseconds = 30,
    .windowdurationmilliseconds = 10,
    .partitionperiodstart = true,
    },{
    .windowidentifier = 202,
    .windowstartmilliseconds = 130,
    .windowdurationmilliseconds = 10,
    .partitionperiodstart = true,
    },};

const window_schedule windowp_stio_sys[2] = {{
    .windowidentifier = 301,
    .windowstartmilliseconds = 40,
    .windowdurationmilliseconds = 30,
    .partitionperiodstart = true,
    },{
    .windowidentifier = 302,
    .windowstartmilliseconds = 140,
    .windowdurationmilliseconds = 30,
    .partitionperiodstart = true,
    },};

const window_schedule windowp_evil[1] = {{
    .windowidentifier = 401,
    .windowstartmilliseconds = 180,
    .windowdurationmilliseconds = 20,
    .partitionperiodstart = true,
    },};

const partition_schedule partition_schedules[4] = {{
    .partitionidentifier = 1,
    .partitionname = "dummy_1",
    .peroidseconds = 0.100,
    .perioddurationseconds = 0.020,
    .numWindows = 2,
    .window_arr = windowp_dummy_1,
    },{
    .partitionidentifier = 2,
    .partitionname = "dummy_2",
    .peroidseconds = 0.100,
    .perioddurationseconds = 0.010,
    .numWindows = 2,
    .window_arr = windowp_dummy_2,
    },{
    .partitionidentifier = 3,
    .partitionname = "stio_sys",
    .peroidseconds = 0.100,
    .perioddurationseconds = 0.030,
    .numWindows = 2,
    .window_arr = windowp_stio_sys,
    },{
    .partitionidentifier = 4,
    .partitionname = "evil",
    .peroidseconds = 0.200,
    .perioddurationseconds = 0.020,
    .numWindows = 1,
    .window_arr = windowp_evil,
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

const uint32_t majorFrameSeconds = 200;