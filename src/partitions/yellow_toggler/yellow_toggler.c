#include <stdio.h>
#include <string.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <drivers/uart.h>

#include <apex_queuing.h>


static void worker(void);


void yellow_toggler_main(void)
{
    PROCESS_ATTRIBUTE_TYPE attributes = {
        .PERIOD = 0,
        .TIME_CAPACITY = 0,
        .ENTRY_POINT = &worker,
        .STACK_SIZE = 1024,
        .BASE_PRIORITY = 1,
        .DEADLINE = SOFT,
        .NAME = "worker",
    };
    
    PROCESS_ID_TYPE processId;
    RETURN_CODE_TYPE RETURN_CODE;
    CREATE_PROCESS(&attributes, &processId, &RETURN_CODE);
    STOP_SELF();

    while(1);
}

void worker(void)
{
    QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
    RETURN_CODE_TYPE RETURN_CODE;
    CREATE_QUEUING_PORT("yellow_print", 32, 32, SOURCE, FIFO,
		&QUEUING_PORT_ID, &RETURN_CODE);


    while (1) {
        char *str = "Yellow toggler\n\r";
        size_t len = strlen(str);
        RETURN_CODE_TYPE RETURN_CODE;
        SEND_QUEUING_MESSAGE(QUEUING_PORT_ID, (uint8_t *)str,
			len, 0,	&RETURN_CODE);

        for (size_t i = 0; i < 8; i++) {
            onboard_led_toggle(yellow_led);
            delay_ms(100);
        }
    }
}
