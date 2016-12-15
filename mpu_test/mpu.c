#include <stm32f4xx_hal.h>

#include "mpu.h"

uint32_t memory[NR_PARTITIONS];
uint32_t located[NR_PARTITIONS];
uint32_t adress[NR_PARTITIONS];
uint8_t vec[6] = {0, 0, 0, 0, 0, 0};
memoryData info[NR_PARTITIONS];
uint8_t nr_located = 0;

void init_mpuOS(void)
{
	HAL_MPU_Disable();

	MPU_Region_InitTypeDef MPU_InitStruct = {
		.Enable = MPU_REGION_ENABLE,
		.Number = MPU_REGION_NUMBER0,
		.BaseAddress = 0x20896000,
		.Size = MPU_16KB,
		.SubRegionDisable = 0x00,
		.TypeExtField = MPU_TEX_LEVEL0,
		.AccessPermission = MPU_REGION_NO_ACCESS,
		.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE,
		.IsShareable = MPU_ACCESS_NOT_SHAREABLE,
		.IsCacheable = MPU_ACCESS_NOT_CACHEABLE,
		.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE,
	};
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.BaseAddress = 0x20000000;
    MPU_InitStruct.Size = MPU_16KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void init_mpu(part_mem_t data, uint8_t partition){

    MPU_Region_InitTypeDef MPU_InitStruct = {
		.Enable = MPU_REGION_ENABLE,
		.Number = regNr(info[partition]),
		.BaseAddress = data.memory_arr[1].address,
		.Size = area(info[partition].region),
		.SubRegionDisable = codeConvert(info[partition]),
		.TypeExtField = MPU_TEX_LEVEL0,
		.AccessPermission = MPU_REGION_NO_ACCESS,
		.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE,
		.IsShareable = MPU_ACCESS_NOT_SHAREABLE,
		.IsCacheable = MPU_ACCESS_NOT_CACHEABLE,
		.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE,
	};
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

void sortMem(part_mem_t *data[3], uint32_t pointer, uint8_t memBool){

    uint8_t i, j, num_free, nr_region = 0;
    uint32_t sub_sec, regionSize;

    for(i=0; i<NR_PARTITIONS; i++)
        located[i] = 0;

    for(i=0; i<NR_PARTITIONS; i++){
        if(memBool == 0)
            memory[i] = data[i]->memory_arr[0].size;
        else
            memory[i] = data[i]->memory_arr[1].size;
    }
	while(nr_located != NR_PARTITIONS){
		regionSize = 0;
		for(i=0; i<NR_PARTITIONS; i++){
		    if(located[i] == 0 && regionSize < memory[i]){
		        j = i;
		        regionSize = memory[i];
		    }
		}
		nr_region++;
		regionSize = 1;
		while(memory[j] > regionSize)
		    regionSize = regionSize * 2;
		sub_sec = regionSize/8;
		located[j] = memAloc(memory[j], sub_sec);
		adress[j] = pointer;
		num_free = (regionSize - memory[j]) / sub_sec;
		updateMem(j, pointer, sub_sec, regionSize, 8 - num_free, 1, nr_region);
		if(num_free > 0){
		    insertMem(num_free, j, sub_sec, regionSize, nr_region);
		}
		nr_located++;
		pointer = pointer + regionSize;
	}
    for(i=0; i<NR_PARTITIONS; i++){
        if(memBool == 0)
            data[i]->memory_arr[0].address = adress[i];
        else
            data[i]->memory_arr[1].address = adress[i];
    }

    for(i=0; i<NR_PARTITIONS; i++){
        if(memBool == 0)
            data[i]->memory_arr[0].size = located[i];
        else
            data[i]->memory_arr[1].size = located[i];
    }

}

uint8_t memAloc(uint8_t mem, uint32_t sub_sec){

	int aux;

	aux = mem / sub_sec;
	if(mem % sub_sec != 0)
		aux++;

	return aux * sub_sec;
}

void insertMem(uint8_t num_free, uint8_t partition, uint32_t sub_sec, uint8_t region, uint8_t nr_region){

	uint8_t i, perc[6] = {0, 0, 0, 0, 0, 0};

	switch(num_free){
        case 1:
            for(i=0; i<NR_PARTITIONS; i++){
                if(located[i] == 0 && memory[i] <= sub_sec){
                    if(perc[0] < memory[i]){
                        vec[0] = i;
                        perc[0] = memory[i];
                    }
                }
            }
            if(perc[0] != 0){
                updateMem(vec[0], adress[partition] + located[partition], sub_sec, region, 1, 8, nr_region);
                nr_located++;
            }
            break;

        case 2:
            for(i=0; i<NR_PARTITIONS; i++)
                if(located[i] == 0 && memory[i] <= sub_sec * 2)
                    if(perc[0] < memory[i]){
                        vec[0] = i;
                        perc[0] = memory[i];
                    }
            for(i=0; i<NR_PARTITIONS; i++)
                if(located[i] == 0 && memory[i] <= sub_sec){
                    if(perc[1] < memory[i]){
                        vec[2] = vec[1];
                        perc[2] = perc[1];
                        vec[1] = i;
                        perc[1] = memory[i];
                    }
                    else
                        if(perc[2] < memory[i]){
                            vec[2] = i;
                            perc[2] = memory[i];
                        }
                }
            if(perc[0] != 0 && perc[0] >= perc[1] + perc[2]){
                updateMem(vec[0], adress[partition] + located[partition], sub_sec, region, 2, 7, nr_region);
                nr_located++;
            }
            else
                if(perc[1] != 0){
                    updateMem(vec[1], adress[partition] + located[partition], sub_sec, region, 1, 7, nr_region);
                    nr_located++;
                }
                if(perc[2] != 0){
                    updateMem(vec[2], adress[vec[1]] + sub_sec, sub_sec, region, 1, 8, nr_region);
                    nr_located++;
                }
            break;

        case 3:
            for(i=0; i<NR_PARTITIONS; i++)
                if(located[i] == 0 && memory[i] <= sub_sec * 3)
                    if(perc[0] < memory[i]){
                        vec[0] = i;
                        perc[0] = memory[i];
                    }
            for(i=0; i<NR_PARTITIONS; i++)
                if(located[i] == 0 && memory[i] <= sub_sec * 2){
                        if(memory[i] <= sub_sec){
                            if(perc[1] < memory[i]){
                                vec[1] = i;
                                perc[1] = memory[i];
                            }
                        }
                        else
                            if(perc[2] < memory[i]){
                                vec[2] = i;
                                perc[2] = memory[i];
                            }
                }
            for(i=0; i<NR_PARTITIONS; i++)
                if(located[i] == 0 && memory[i] <= sub_sec){
                    if(perc[3] < memory[i]){
                        vec[5] = vec[4];
                        perc[5] = perc[4];
                        vec[4] = vec[3];
                        perc[4] = perc[3];
                        vec[3] = i;
                        perc[3] = memory[i];
                    }
                    else
                        if(perc[4] < memory[i]){
                            vec[5] = vec[4];
                            perc[5] = perc[4];
                            vec[4] = i;
                            perc[4] = memory[i];
                        }
                        else
                            if(perc[5] < memory[i]){
                                vec[5] = i;
                                perc[5] = memory[i];
                            }
                }
            if(perc[0] != 0 && perc[0] >= perc[1] + perc[2] && perc[0] >= perc[3] + perc[4] + perc[5]){
                updateMem(vec[0], adress[partition] + located[partition], sub_sec, region, 3, 6, nr_region);
	            nr_located++;
            }
            else{
                if(perc[1] + perc[2] >= perc[3] + perc[4] + perc[5]){
                    if(perc[1] != 0){
                        updateMem(vec[1], adress[partition] + located[partition], sub_sec, region, 1, 6, nr_region);
                        nr_located++;
                        if(perc[2] != 0){
                            updateMem(vec[2], adress[vec[1]] + sub_sec, sub_sec, region, 2, 7, nr_region);
                            nr_located++;
                        }
                        else{
                            updateMem(vec[1], adress[partition] + located[partition], sub_sec, region, 3, 6, nr_region);
                        }
                    }
                    else
                        if(perc[2] != 0){
                            updateMem(vec[2], adress[partition] + located[partition], sub_sec, region, 3, 6, nr_region);
                            nr_located++;
                        }
                }
                else{
                    if(perc[3] != 0){
                        updateMem(vec[3], adress[partition] + located[partition], sub_sec, region, 1, 6, nr_region);
                        nr_located++;
                    }
                    if(perc[4] != 0){
                        updateMem(vec[4], adress[vec[3]] + sub_sec, sub_sec, region, 1, 7, nr_region);
                        nr_located++;
                    }
                    if(perc[5] != 0){
                        updateMem(vec[5], adress[vec[4]] + sub_sec, sub_sec, region, 1, 8, nr_region);
                        nr_located++;
                    }
                }
            }

            break;
        default:
            break;
	}
}


void updateMem(uint8_t partition, uint8_t pointer, uint32_t sub_sec, uint8_t region, uint8_t nr_sec, uint8_t first, uint8_t nr_region){

        adress[partition] = pointer;
        located[partition] = sub_sec * nr_sec;
        info[partition].region = region;
        info[partition].nr_sec = nr_sec;
        info[partition].first = first;
        info[partition].nr_region = nr_region;
}


MPU_REGION_SIZE area(uint32_t size){

    switch(size){
        case 32:
            return MPU_32B;
            break;
        case 64:
            return MPU_64B;
            break;
        case 128:
            return MPU_128B;
            break;
        case 256:
            return MPU_256B;
            break;
        case 512:
            return MPU_512B;
            break;
        case 1024:
            return MPU_1KB;
            break;
        case 2048:
            return MPU_2KB;
            break;
        case 4096:
            return MPU_4KB;
            break;
        case 8192:
            return MPU_8KB;
            break;
        case 16384:
            return MPU_16KB;
            break;
        case 32768:
            return MPU_32KB;
            break;
        case 65536:
            return MPU_64KB;
            break;
        case 131072:
            return MPU_128KB;
            break;
        default:
            return MPU_128KB;
            break;
    }
    return MPU_128KB;
}


uint8_t codeConvert(memoryData info){

    switch(info.first){
        case 1:
            switch(info.nr_sec){
                case 5:
                    return 0x07;
                    break;
                case 6:
                    break;
                    return 0x03;
                case 7:
                    break;
                    return 0x01;
                case 8:
                    break;
                    return 0x00;
                default:
                    return 0x00;
                    break;
            }
            break;
        case 6:
            switch(info.nr_sec){
                case 1:
                    return 0xFB;
                    break;
                case 2:
                    break;
                    return 0xF9;
                case 3:
                    break;
                    return 0xF8;
                default:
                    return 0xF8;
                    break;
            }
            break;
        case 7:
            switch(info.nr_sec){
                case 1:
                    return 0xFD;
                    break;
                case 2:
                    break;
                    return 0xFC;
                default:
                    return 0xFC;
                    break;
            }
            break;
        case 8:
            switch(info.nr_sec){
                case 1:
                    return 0xFE;
                    break;
                default:
                    return 0xFE;
                    break;
            }
            break;
        default:
            return 0x00;
            break;
    }
    return 0x00;
}

MPU_REGION_NUMBER regNr(memoryData info){

    switch(info.nr_region){
        case 0:
            return REGION_NUMBER0;
            break;
        case 1:
            return REGION_NUMBER1;
            break;
        case 2:
            return REGION_NUMBER2;
            break;
        case 3:
            return REGION_NUMBER3;
            break;
        case 4:
            return REGION_NUMBER4;
            break;
        case 5:
            return REGION_NUMBER5;
            break;
        case 6:
            return REGION_NUMBER6;
            break;
        case 7:
            return REGION_NUMBER7;
            break;
        default:
            break;
    }

    return REGION_NUMBER1;
}
