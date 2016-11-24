#include <stdio.h>
#include "partition.h"

partition_t configured_partitions[MAX_PARTITION];
static uint8_t n_partitions = 1;

partition_t* partition_create(partition* partition_)
{
    uint8_t i;
    partition_t* part = &configured_partitions[n_partitions];
    //*part = (partition_t) {0};

    part->partitionInfo = *partition_;
    /*{
        .partitionidentifier = partition_->partitionidentifier,
        .partitionname = partition_->partitionname,
        .criticality = partition_->criticality,
        .systempartion = partition_->systempartion,
        .entrypoint = partition_->entrypoint,
        .queue_arr = partition_->queue_arr,
        .sample_arr = partition_->sample_arr,
    };*/

    for(i = 0; i < MAX_PROCESSES_PER_PARTITIONS; i++)
    {
        part->processes[i] = (process_t) {0};
    }

    return part;
}

uint8_t partition_getNumberOfPartitions(void)
{
    return n_partitions;
}