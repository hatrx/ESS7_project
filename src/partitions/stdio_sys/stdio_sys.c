#include <stdio.h>
#include <stdint.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>


void stdio_sys_main(void)
{
#if 1
	while(1);
#else
	while (1) {
		int8_t c;
		if (!pop(&c)) {
			putchar(c);
			//onboard_led_toggle(red_led);
		}
	}
#endif
}
