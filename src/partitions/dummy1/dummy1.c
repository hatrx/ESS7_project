#include <stdio.h>
#include <string.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>

#include <apex_queuing.h>


void dummy1_main(void)
{
	QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
	RETURN_CODE_TYPE RETURN_CODE;
	CREATE_QUEUING_PORT("print_1", 32, 32, SOURCE, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);


	while (1) {
		char *str = "Dummy 1\n\r";
		size_t len = strlen(str);
		RETURN_CODE_TYPE RETURN_CODE;
		SEND_QUEUING_MESSAGE(QUEUING_PORT_ID, (uint8_t *)str, len, 0, &RETURN_CODE);

		for (size_t i = 0; i < 8; i++) {
			onboard_led_toggle(yellow_led);
			delay_ms(100);
		}
	}
}
