#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <apex_process.h>
#include <string.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/get_time.h"
#include "drivers/rtc.h"
#include "drivers/utils.h"

#include "kernel/context.h"
#include "kernel/arinc/partition.h"
#include "kernel/context.h"
#include "kernel/queuing_port.h"

//#include "hedder.h"
#include "kernel/statics.h"
//void dummy1_main(void);
//void dummy2_main(void);


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
	PROCESS_ATTRIBUTE_TYPE dummy1_mainProcess_attributes, dummy2_mainProcess_attributes;
	PROCESS_ID_TYPE dummy1_pid, dummy2_pid;

	set_system_clock_168mhz();

	if (BSP_UART_Init() != 0)
	{
		Error_Handler();		// Shit not working!
	}

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

/*
	if (BSP_IWDG_Init() != 0)		//Independent watchdog time initilisation
	{
		Error_Handler();		// Shit not working!
	}

	if (BSP_RTC_Init() != 0)		//Real time clock initialisation. Define the time interval in the header file
	{
		Error_Handler();		// Shit not working!
	}
*/
	//init_mpu(0x20000000 + 0x2000, MPU_1KB);

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

	int_partitions();

	process_createProcess(&test_partitions[0], 0x20001000, &dummy1_mainProcess_attributes, &dummy1_pid);
	process_createProcess(&test_partitions[1], 0x20003000, &dummy2_mainProcess_attributes, &dummy2_pid);

	//setup_contexts(&dummy1_main, (void *)0x20001000);
	//setup_contexts(&dummy2_main, (void *)0x20003000);

#if 0
	/* Initialize all ports */
	init_queuing_ports();

	{
		/* Change partition to set up port */
		curr_partition_id = 1;

		QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
		RETURN_CODE_TYPE RETURN_CODE;
		CREATE_QUEUING_PORT("print_1", 8, 30, SOURCE, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Create port error!\n");
		} else {
			printf("activated port '%s' with ID: %2lu\n", "print_1", QUEUING_PORT_ID);
		}
	}


	{
		/* Change partition to set up port */
		curr_partition_id = 1;

		QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
		RETURN_CODE_TYPE RETURN_CODE;
		GET_QUEUING_PORT_ID("print_1", &QUEUING_PORT_ID, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Get port ID error!\n");
		} else {
			printf("Got ID: %2lu of port '%s'\n\n", QUEUING_PORT_ID, "print_1");
		}
	}


	{
		/* Change partition to set up port */
		curr_partition_id = 3;

		QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
		RETURN_CODE_TYPE RETURN_CODE;
		CREATE_QUEUING_PORT("sys_stio", 8, 30, DESTINATION, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Create port error!\n");
		} else {
			printf("activated port '%s' with ID: %2lu\n", "sys_stio", QUEUING_PORT_ID);
		}
	}


	{
		/* Change partition to set up port */
		curr_partition_id = 3;

		QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
		RETURN_CODE_TYPE RETURN_CODE;
		GET_QUEUING_PORT_ID("sys_stio", &QUEUING_PORT_ID, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Get port ID error!\n");
		} else {
			printf("Got ID: %2lu of port '%s'\n\n", QUEUING_PORT_ID, "sys_stio");
		}
	}


	{
		/* Change partition to send message */
		curr_partition_id = 1;

		RETURN_CODE_TYPE RETURN_CODE;
		char *str = "Holy cow";
		size_t len = strlen(str);
		SEND_QUEUING_MESSAGE(0, (uint8_t *)str, len, 0, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Push error!\n");
		}
	}


	{
		/* Change partition to recieve message */
		curr_partition_id = 3;

		RETURN_CODE_TYPE RETURN_CODE;
		QUEUING_PORT_STATUS_TYPE QUEUING_PORT_STATUS;
		GET_QUEUING_PORT_STATUS(0, &QUEUING_PORT_STATUS, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Status error!\n");
		}
		printf("NB_MESSAGE: %2lu\n", QUEUING_PORT_STATUS.NB_MESSAGE);
	}


	{
		/* Change partition to recieve message */
		curr_partition_id = 3;

		RETURN_CODE_TYPE RETURN_CODE;
		char str[10] = {0};
		MESSAGE_SIZE_TYPE len;
		RECEIVE_QUEUING_MESSAGE(0, 0, (uint8_t *)str, &len, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Pop error!\n");
		}
		printf("Popped string: %s\n", str);
		printf("String length: %lu\n", len);
	}


	{
		/* Change partition to recieve message */
		curr_partition_id = 3;

		RETURN_CODE_TYPE RETURN_CODE;
		QUEUING_PORT_STATUS_TYPE QUEUING_PORT_STATUS;
		GET_QUEUING_PORT_STATUS(0, &QUEUING_PORT_STATUS, &RETURN_CODE);
		if (RETURN_CODE != NO_ERROR) {
			printf("Status error!\n");
		}
		printf("NB_MESSAGE: %2lu\n", QUEUING_PORT_STATUS.NB_MESSAGE);
	}
#endif

	//uint32_t counter = 0;

	while (1) {
		//printf("Hello world! : %"PRIu32"\n", counter++);
		HAL_Delay(1000);

	}
}
