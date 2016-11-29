#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/get_time.h"
#include "drivers/utils.h"

#include "kernel/context.h"
#include "kernel/statics.h"
#include "kernel/queuing_port.h"


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


void SVC_Handler(void)
{
	printf("SVC_Handler\n");
}


int main(void)
{
	set_system_clock_168mhz();

	HAL_Init();
	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

	//time slot for the watchdog to be refreshed in (in miliseconds)
	//BSP_IWDG_init(3000);

	//init_mpu(0x20000000 + 0x2000, MPU_1KB);

	setup_contexts(&dummy1_main, (void *)0x20001000);
	setup_contexts(&dummy2_main, (void *)0x20003000);
	setup_contexts(&stdio_sys_main, (void *)0x20005000);


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


	uint32_t counter = 0;

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);
		HAL_Delay(1000);
		//BSP_IWDG_refresh();
	}
}
