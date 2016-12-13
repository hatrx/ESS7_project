#ifndef MPU_H
#define MPU_H


#include <stdint.h>


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


void init_mpu(uint32_t base_addr, MPU_REGION_SIZE);


#endif /* MPU_H */
