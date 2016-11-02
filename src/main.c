#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "uart.h"
#include "mpu.h"
#include "onboard_leds.h"
#include "system_clock.h"


int main(void)
{
	UART_HandleTypeDef UartHandle;

	HAL_Init();
	set_system_clock_168mhz();
	HAL_UART_MspInit(&UartHandle);

	init_onboard_LED(red_led);

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	uint32_t counter = 0;

	init_mpu(0x20000000 + 0x2000, MPU_1KB);

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);
		onboard_led_toggle(red_led);
		HAL_Delay(1000);
	}
}
