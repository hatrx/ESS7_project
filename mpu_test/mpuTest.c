#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define NR_PARTITIONS 5
#define HIGH 3200
#define LENGHT 200

typedef struct _memoryData {

    uint8_t nr_region;
    uint32_t region;
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

typedef struct {
	uint32_t                  size;
	uint32_t                  address;
} mem_req_t;

typedef struct{
	uint32_t                  arr_size;
	mem_req_t                 *memory_arr;
} part_mem_t;


mem_req_t mem_dummy_1[2] = {{
	.size = 20,
	.address = 0x8010000
},{
	.size = 20,
	.address = 0x20010000
},};

mem_req_t mem_dummy_2[2] = {{
	.size = 65,
	.address = 0x8012000
},{
	.size = 65,
	.address = 0x20012000
},};

mem_req_t mem_stdio_sys[2] = {{
	.size = 120,
	.address = 0x8014000
},{
	.size = 120,
	.address = 0x20014000
},};

mem_req_t mem_dummy_3[2] = {{
	.size = 80,
	.address = 0x8010000
},{
	.size = 80,
	.address = 0x20010000
},};

mem_req_t mem_dummy_4[2] = {{
	.size = 40,
	.address = 0x8012000,
},{
	.size = 40,
	.address = 0x20012000,
},};

part_mem_t partition_memmory[NR_PARTITIONS] = {{
	.arr_size = 2,
	.memory_arr = mem_dummy_1
},{
	.arr_size = 2,
	.memory_arr = mem_dummy_2
},{
	.arr_size = 2,
	.memory_arr = mem_stdio_sys,
},{
	.arr_size = 2,
	.memory_arr = mem_dummy_3
},{
	.arr_size = 2,
	.memory_arr = mem_dummy_4
},};

uint32_t memory[NR_PARTITIONS];
uint32_t located[NR_PARTITIONS];
uint32_t adress[NR_PARTITIONS];
uint8_t vec[6] = {0, 0, 0, 0, 0, 0};
memoryData info[NR_PARTITIONS];
uint8_t nr_located = 0;
uint8_t allocated[1280*128];
uint32_t z, auxiliar=0;

void updateMem(uint8_t partition, uint32_t pointer, uint32_t sub_sec, uint32_t region, uint8_t nr_sec, uint8_t first, uint8_t nr_region){

        adress[partition] = pointer;
        located[partition] = sub_sec * nr_sec;
        info[partition].region = region;
        info[partition].nr_sec = nr_sec;
        info[partition].first = first;
        info[partition].nr_region = nr_region;
        printf("   Partition: %d Located: %d Adress: %x\n", partition, located[partition], adress[partition]);
        for(z=0; z<sub_sec*nr_sec; z++)
            allocated[z+auxiliar] = partition;
        auxiliar = auxiliar + sub_sec * nr_sec;

}

uint32_t memAloc(uint32_t mem, uint32_t sub_sec){

	int aux;

	aux = mem / sub_sec;
	if(mem % sub_sec != 0)
		aux++;
	return aux * sub_sec;
}

void insertMem(uint8_t num_free, uint8_t partition, uint32_t sub_sec, uint32_t region, uint8_t nr_region){

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



void sortMem(part_mem_t partition_memmory[NR_PARTITIONS], uint32_t pointer, uint8_t memBool){

	uint8_t i, j, num_free, nr_region = 0;
	uint32_t regionSize, sub_sec;

    for(i=0; i<NR_PARTITIONS; i++)
        located[i] = 0;

    for(i=0; i<NR_PARTITIONS; i++){
        if(memBool == 0)
            memory[i] = partition_memmory[i].memory_arr[0].size;
        else
            memory[i] = partition_memmory[i].memory_arr[1].size;
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
		printf("\n  Region %d size: %d\n", nr_region, regionSize);
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
            partition_memmory[i].memory_arr[0].address = adress[i];
        else
            partition_memmory[i].memory_arr[1].address = adress[i];
    }

    for(i=0; i<NR_PARTITIONS; i++){
        if(memBool == 0)
            partition_memmory[i].memory_arr[0].size = located[i];
        else
            partition_memmory[i].memory_arr[1].size = located[i];
    }
}

int main(int argc, char const *argv[]) {

    int i, j;

    printf("\n  Initial pointer: 0x0200000\n\n");

    for(i=0; i<NR_PARTITIONS; i++){
        printf("  Partition: %d - Initial size: %d\n", i, partition_memmory[i].memory_arr[1].size);
    }

	sortMem(partition_memmory, 0x0200000, 1);

    printf("\n");
    for(i=0; i<NR_PARTITIONS; i++){
        printf("  Partition : %d\n  Memory reserved: %d\n  Starting pointer: %x\n\n", i, partition_memmory[i].memory_arr[1].size, partition_memmory[i].memory_arr[1].address);
    }

    for(i=0; i<12; i++){
     printf("  ");
        for(j=0; j<32; j++)
            printf("%d", allocated[i*32 + j]);
        printf("\n");
    }

    printf("\n  Memory allocated: %d\n\n", auxiliar);

	return 0;
}
