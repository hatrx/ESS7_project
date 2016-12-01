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
#include "drivers/time_get.h"
#include "drivers/rtc.h"
#include "drivers/utils.h"

#include "kernel/context.h"
#include "kernel/context.h"

#include "kernel/arinc/partition.h"
#include "kernel/arinc/statics.h"
#include "kernel/arinc/queuing_port.h"

void UsageFault_Handler(void)
{
	printf("UsageFault_Handler\n");
}


void HardFault_Handler(void)
{
	printf("HardFault_Handler\n");
	SCB_Type *something = SCB;
	uint32_t hfsr = something->HFSR;
	uint32_t cfsr = something->CFSR;
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
	/* Disable sysTick */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	PROCESS_ATTRIBUTE_TYPE dummy1_mainProcess_attributes, dummy2_mainProcess_attributes, stdio_sys_mainProcess_attributes;
	PROCESS_ID_TYPE dummy1_pid, dummy2_pid, stdio_sys_pid;

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
	/* Initialize all ports */
	init_queuing_ports();

	/* Enable sysTick */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	dummy1_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE) {
		.PERIOD = 0,
		.TIME_CAPACITY = 0,
		.ENTRY_POINT = &dummy1_main,
		.BASE_PRIORITY = 1,
		.DEADLINE = SOFT,
		.NAME = "dummy1",
	};

	dummy2_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE) {
		.PERIOD = 0,
		.TIME_CAPACITY = 0,
		.ENTRY_POINT = &dummy2_main,
		.BASE_PRIORITY = 1,
		.DEADLINE = SOFT,
		.NAME = "dummy2",
	};

	stdio_sys_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE) {
		.PERIOD = 0,
		.TIME_CAPACITY = 0,
		.ENTRY_POINT = &stdio_sys_main,
		.BASE_PRIORITY = 1,
		.DEADLINE = SOFT,
		.NAME = "stdio_sys",
	};
	int_partitions();
	process_createProcess(&test_partitions[0], 0x20001000, &dummy1_mainProcess_attributes, &dummy1_pid);
	process_createProcess(&test_partitions[1], 0x20003000, &dummy2_mainProcess_attributes, &dummy2_pid);
	process_createProcess(&test_partitions[2], 0x20005000, &stdio_sys_mainProcess_attributes, &stdio_sys_pid);
	TIME_Start_ns();


	while (1) {
	}
}
