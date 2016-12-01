#include <stdio.h>
#include <stddef.h>
#include "partition.h"
#include "statics.h"


void init_partitions(void) {
	const uint32_t nb_processes = sizeof(test_partitions) / sizeof(partition_t);
	for (size_t i = 0; i < nb_processes; i++) {
		test_partitions[i].nb_processes = 0;
	}
}
