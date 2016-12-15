#ifndef MPU_H
#define MPU_H

#include "statics.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define NR_PARTITIONS 3

typedef struct _memoryData {

    uint8_t nr_region;
    uint8_t region;
    uint8_t nr_sec;
    uint8_t first;

} memoryData;


typedef enum {
	MPU_32B   = (uint8_t)0x04,
	MPU_64B   = (uint8_t)0x05,
	MPU_128B  = (uint8_t)0x06,
	MPU_256B  = (uint8_t)0x07,
	MPU_512B  = (uint8_t)0x08,
	MPU_1KB   = (uint8_t)0x09,
	MPU_2KB   = (uint8_t)0x0A,
	MPU_4KB   = (uint8_t)0x0B,
	MPU_8KB   = (uint8_t)0x0C,
	MPU_16KB  = (uint8_t)0x0D,
	MPU_32KB  = (uint8_t)0x0E,
	MPU_64KB  = (uint8_t)0x0F,
	MPU_128KB = (uint8_t)0x10,
} MPU_REGION_SIZE;

typedef enum {
	REGION_NUMBER0  =  ((uint8_t)0x00),
	REGION_NUMBER1  =  ((uint8_t)0x01), 
	REGION_NUMBER2  =  ((uint8_t)0x02),  
	REGION_NUMBER3  =  ((uint8_t)0x03),  
	REGION_NUMBER4  =  ((uint8_t)0x04), 
	REGION_NUMBER5  =  ((uint8_t)0x05),
	REGION_NUMBER6  =  ((uint8_t)0x06),
	REGION_NUMBER7  =  ((uint8_t)0x07),

} MPU_REGION_NUMBER;

void init_mpuOS(void);
MPU_REGION_SIZE area(uint32_t size);
uint8_t codeConvert(memoryData info);
void init_mpu(part_mem_t data, uint8_t partition);
uint8_t memAloc(uint8_t mem, uint32_t sub_sec);
void insertMem(uint8_t num_free, uint8_t partition, uint32_t sub_sec, uint8_t region, uint8_t nr_region);
void sortMem(part_mem_t *data[3], uint32_t pointer, uint8_t memBool);
void updateMem(uint8_t partition, uint8_t pointer, uint32_t sub_sec, uint8_t region, uint8_t nr_sec, uint8_t first, uint8_t nr_region);
MPU_REGION_NUMBER regNr(memoryData info);


#endif /* MPU_H */
