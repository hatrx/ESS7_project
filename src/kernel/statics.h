#ifndef STATICS_H
#define STATICS_H

#include "types.h"
#include "xml_data.h"

//extern partition_t partitions[3];

	/* This value could be calculated every time we make a new process,
	   but this is just way easier... */
	uint32_t                  mem_offset;
extern channel_t connection_table[1];
extern part_mem_t partition_memory[4];

#endif /* STATICS_H */
