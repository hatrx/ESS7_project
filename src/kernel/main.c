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




	uint32_t counter = 0;

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);
		HAL_Delay(1000);
		//BSP_IWDG_refresh();
	}
}
