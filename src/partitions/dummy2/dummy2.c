#include <drivers/utils.h>
#include <drivers/onboard_leds.h>


void dummy2_main(void)
{
	while (1) {
		onboard_led_toggle(red_led);
		delay_ms(50);
	}
}
