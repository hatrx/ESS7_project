#include <stdio.h>
#include <inttypes.h>
#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "dummy1.h"

void dummy1_main(void)
{
    uint32_t counter = 0;
    while (1)
    {
        printf("Dummy1 : %"PRIu32"\n", counter++);
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}