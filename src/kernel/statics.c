#include <stdint.h>

#include "circular_buffer.h"
#include "../xml_data.h"
#include "ports.h"

mem_req_t mem_dummy_1[2] = {{
    .type = CODE,
    .size = 8000,
    .access = READ_ONLY,
    .address = 0x8010000,
},{
    .type = DATA,
    .size = 8000,
    .access = READ_WRITE,
    .address = 0x20010000,
},};

mem_req_t mem_dummy_2[2] = {{
    .type = CODE,
    .size = 8000,
    .access = READ_ONLY,
    .address = 0x8012000,
},{
    .type = DATA,
    .size = 8000,
    .access = READ_WRITE,
    .address = 0x20012000,
},};

mem_req_t mem_stdio_sys[2] = {{
    .type = CODE,
    .size = 8000,
    .access = READ_ONLY,
    .address = 0x8014000,
},{
    .type = DATA,
    .size = 8000,
    .access = READ_WRITE,
    .address = 0x20014000,
},};

mem_req_t mem_evil[2] = {{
    .type = CODE,
    .size = 8000,
    .access = READ_ONLY,
    .address = 0x8016000,
},{
    .type = DATA,
    .size = 8000,
    .access = READ_WRITE,
    .address = 0x20016000,
},};

part_mem_t partition_memory[4] = {{
    .id = 1,
    .partitionname = "dummy_1",
    .arr_size = 2,
    .memory_arr = mem_dummy_1,
    .mem_offset = 1024, /* TODO: This could be initialized at memeory init. */
},{
    .id = 2,
    .partitionname = "dummy_2",
    .arr_size = 2,
    .memory_arr = mem_dummy_2,
    .mem_offset = 1024, /* TODO: This could be initialized at memeory init. */
},{
    .id = 3,
    .partitionname = "stio_sys",
    .arr_size = 2,
    .memory_arr = mem_stdio_sys,
    .mem_offset = 1024, /* TODO: This could be initialized at memeory init. */
},{
    .id = 4,
    .partitionname = "evil",
    .arr_size = 2,
    .memory_arr = mem_evil,
    .mem_offset = 1024,
},};

/*port_t *stio_channel_ports[3] = {
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
};*/
