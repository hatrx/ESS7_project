#include <stdio.h>
#include <string.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <apex_types.h>
#include <apex_time.h>

#include <apex_queuing.h>


void dummy2_main(void)
{
	QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
	RETURN_CODE_TYPE RETURN_CODE;
	CREATE_QUEUING_PORT("print_2", 32, 32, SOURCE, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);
	SYSTEM_TIME_TYPE timeNow;
	RETURN_CODE_TYPE returnCode;

	while (1) {
		char *str = "Dummy 2\n\r";
		size_t len = strlen(str);
		RETURN_CODE_TYPE RETURN_CODE;
		SEND_QUEUING_MESSAGE(QUEUING_PORT_ID, (uint8_t *)str, len, 0, &RETURN_CODE);

		for (size_t i = 0; i < 8; i++) {
			onboard_led_toggle(red_led);
			delay_ms(100);
		}
		GET_TIME(&timeNow, &returnCode);
	}
}
