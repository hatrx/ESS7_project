#include <stm32f4xx_hal.h>

#include "dummy1.h"


void dummy1_main(void)
{
	while (1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
		HAL_Delay(50);
	}
}
