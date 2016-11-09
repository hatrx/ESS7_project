#include <stm32f4xx_hal.h>

#include "dummy2.h"


void dummy2_main(void)
{
	while (1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(1000);
	}
}
