#include <stdio.h>
#include <string.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <apex_types.h>
#include <apex_time.h>

#include <apex_queuing.h>


static void worker(void);


void red_toggler_main(void)
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
    CREATE_QUEUING_PORT("red_print", 32, 32, SOURCE, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);
    SYSTEM_TIME_TYPE timeNow;
    RETURN_CODE_TYPE returnCode;

    while (1) {
        char *str = "  Red toggler\n\r";
        size_t len = strlen(str);
        RETURN_CODE_TYPE RETURN_CODE;
        SEND_QUEUING_MESSAGE(QUEUING_PORT_ID, (uint8_t *)str, len, 0, &RETURN_CODE);

        for (size_t i = 0; i < 10; i++) {
            onboard_led_toggle(red_led);
            delay_ms(100);
        }
        GET_TIME(&timeNow, &returnCode);
    }
}
