#include <stm32f4xx_hal.h>

#include "onboard_leds.h"


void init_onboard_LED(ONBOARD_LED led)
{
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = led,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_LOW,
    };

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void onboard_led_on(ONBOARD_LED led)
{
    HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_SET);
}


void onboard_led_off(ONBOARD_LED led)
{
    HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_RESET);
}


void onboard_led_toggle(ONBOARD_LED led)
{
    HAL_GPIO_TogglePin(GPIOC, led);
}
