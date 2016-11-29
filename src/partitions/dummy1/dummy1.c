#include <string.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>


void dummy1_main(void)
{
#if 1
	while(1);
#else
	const char *str = "Dummy 1, sending.\n\r";

	while (1) {
		for (size_t i = 0; i < strlen(str); i++) {
			push(str[i]);
		}

		for (size_t i = 0; i < 20; i++) {
			onboard_led_toggle(yellow_led);
			delay_ms(50);
		}
	}
#endif
}
