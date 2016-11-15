#include <stm32f4xx_hal.h>

#include "utils.h"


void delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}
