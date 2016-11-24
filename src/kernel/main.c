#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <apex_process.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/get_time.h"
#include "drivers/rtc.h"

#include "kernel/context.h"

#include "kernel/arinc/partition.h"

#include "hedder.h"

void dummy1_main(void);
void dummy2_main(void);

void UsageFault_Handler(void)
{
	printf("UsageFault_Handler\n");
}


void HardFault_Handler(void)
{
	printf("HardFault_Handler\n");
	// SCB_Type *something = SCB;
	// uint32_t hfsr = something->HFSR;
	// uint32_t cfsr = something->CFSR;
	while(1);
}


void MemManager_Handler(void)
{
	printf("MemManager_Handler\n");
}

void Error_Handler(void)
{
	printf("Initialisation Error\n");
	while(1);
}


int main(void)
{
	partition dummy1, dummy2;
	partition_t *dummy1_partition, *dummy2_partition;
	PROCESS_ATTRIBUTE_TYPE dummy1_mainProcess_attributes, dummy2_mainProcess_attributes;
	PROCESS_ID_TYPE dummy1_pid, dummy2_pid;

	set_system_clock_168mhz();
	int x = -1;
	if (BSP_UART_init() != 0)
	{
		Error_Handler();		// Shit not working!
	}

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);
	//BSP_IWDG_init(3000);		//time slot for the watchdog to be refreshed in (in miliseconds)

	if (BSP_RTC_init() != 0)
	{
		Error_Handler();		// Shit not working!
	}

	//init_mpu(0x20000000 + 0x2000, MPU_1KB);

	dummy1 = (partition)
	{
		.partitionidentifier = 1,
		.partitionname = "Dummy1",
		.criticality = "Important",
		.systempartion = false,
		.entrypoint = "dummy1_main",
		.queue_arr = NULL,
		.sample_arr = NULL,
	};

	dummy2 = (partition)
	{
		.partitionidentifier = 2,
		.partitionname = "Dummy2",
		.criticality = "Important",
		.systempartion = false,
		.entrypoint = "dummy2_main",
		.queue_arr = NULL,
		.sample_arr = NULL,
	};

	dummy1_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE)
	{
		.PERIOD = 0,
		.TIME_CAPACITY = 0,
		.ENTRY_POINT = &dummy1_main,
		.BASE_PRIORITY = 1,
		.DEADLINE = SOFT,
		.NAME = "dummy1",
	};
	
	dummy2_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE)
	{
		.PERIOD = 0,
		.TIME_CAPACITY = 0,
		.ENTRY_POINT = &dummy2_main,
		.BASE_PRIORITY = 1,
		.DEADLINE = SOFT,
		.NAME = "dummy2",
	};

	dummy1_partition = partition_create(&dummy1);
	dummy2_partition = partition_create(&dummy2);

	process_createProcess(dummy1_partition, 0x20001000, &dummy1_mainProcess_attributes, &dummy1_pid);
	process_createProcess(dummy2_partition, 0x20003000, &dummy2_mainProcess_attributes, &dummy2_pid);

	//setup_contexts(&dummy1_main, (void *)0x20001000);
	//setup_contexts(&dummy2_main, (void *)0x20003000);

	printf("\n");
	printf("dummy1: %02X\n", &dummy1_main);
	printf("dummy2: %02X\n", &dummy2_main);


	HAL_Init();

	//uint32_t counter = 0;

	while (1) {
		//printf("Hello world! : %"PRIu32"\n", counter++);
		HAL_Delay(1000);

		struct date printdate;
		get_datetime(&printdate);

		printf("%02" PRIu8 ":%02" PRIu8 ":%02" PRIu8 " ", printdate.hours, printdate.minutes, printdate.seconds);
		printf("%02" PRIu8 "-%02" PRIu8 "-%02" PRIu8 " \n", printdate.date, printdate.month, 2000 + printdate.year);
	}
}
