#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <apex_types.h>
#include <apex_time.h>


void dummy2_main(void)
{
	SYSTEM_TIME_TYPE timeNow;
	RETURN_CODE_TYPE returnCode;
	while (1) {
		onboard_led_toggle(red_led);
		delay_ms(50);
		GET_TIME(&returnCode, &timeNow);
	}
}
