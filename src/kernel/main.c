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


int main(void)
{
	HAL_Init();
	set_system_clock_168mhz();

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	BSP_IWDG_init(3000);		//time slot for the watchdog to be refreshed in (in miliseconds)

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

	uint32_t counter = 0;

	init_mpu(0x20000000 + 0x2000, MPU_1KB);

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);
		onboard_led_toggle(red_led);
		HAL_Delay(1000);
		BSP_IWDG_refresh();
		}
}
