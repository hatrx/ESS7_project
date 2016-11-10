#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/get_time.h"

#include "kernel/context.h"

#include "../partitions/dummy1/dummy1.h"
#include "../partitions/dummy2/dummy2.h"


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
	setup_contexts(&dummy1_main, (void *)0x20001000);
	setup_contexts(&dummy2_main, (void *)0x20003000);

	HAL_Init();
	set_system_clock_168mhz();

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	//BSP_IWDG_init(3000);		//time slot for the watchdog to be refreshed in (in miliseconds)

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

	uint32_t counter = 0;

	//init_mpu(0x20000000 + 0x2000, MPU_1KB);

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);

		if(counter%2==0)
		{
			start_time_ms();
			HAL_Delay(1000 + counter * 100);
			uint16_t time_ms = stop_time_ms();
			printf("time: : %d\n", time_ms);
		}
		else
		{
			start_time();
			HAL_Delay(1000 + counter * 100);
			uint32_t time_ns = stop_time();
			printf("time: : %"PRIu32"\n", time_ns);
		}

		onboard_led_toggle(red_led);
		//BSP_IWDG_refresh();
	}
}
