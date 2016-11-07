#include <stdio.h>
#include <inttypes.h>
#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "dummy2.h"

void main(void)
{
    unt32_t counter = 0;
    while (true)
    {
        printf("Dummy2 : %"PRIu32"\n", counter++);
        HAL_Delay(1000);
    }
}