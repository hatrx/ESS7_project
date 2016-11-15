#ifndef ONBOARD_LEDS_H
#define ONBOARD_LEDS_H


#include <stm32f4xx_hal.h>


typedef enum {
	yellow_led = GPIO_PIN_12,
	red_led = GPIO_PIN_13,
} ONBOARD_LED;


void init_onboard_LED(ONBOARD_LED);
void onboard_led_on(ONBOARD_LED);
void onboard_led_off(ONBOARD_LED);
void onboard_led_toggle(ONBOARD_LED);


#endif /* ONBOARD_LEDS_H */
