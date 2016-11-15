#include <drivers/utils.h>
#include <drivers/onboard_leds.h>


void dummy1_main(void)
{
	while (1) {
		onboard_led_toggle(yellow_led);
		delay_ms(50);
	}
}
