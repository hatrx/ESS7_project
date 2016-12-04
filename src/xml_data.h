#ifndef XML_DATA_H
#define XML_DATA_H

#include "kernel/arinc/types.h"

extern partition_t partitions[5];
extern const partition_schedule partition_schedules[4];

extern const uint32_t majorFrameSeconds;

void idle_main(void);

void dummy1_main(void);

void dummy2_main(void);

void stdio_sys_main(void);

void evil_main(void);
#endif
