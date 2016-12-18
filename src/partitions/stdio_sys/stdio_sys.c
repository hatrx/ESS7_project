#include <stdint.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <drivers/uart.h>

#include <apex_queuing.h>


static void worker(void);


void stdio_sys_main(void)
{
    PROCESS_ATTRIBUTE_TYPE attributes = {
        .PERIOD = 0,
        .TIME_CAPACITY = 0,
        .ENTRY_POINT = &worker,
        .STACK_SIZE = 2024,
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
    CREATE_QUEUING_PORT("sys_stio", 32, 32, DESTINATION, FIFO,
		&QUEUING_PORT_ID, &RETURN_CODE);

    char str[1024];
    while(1) {
        RETURN_CODE_TYPE RETURN_CODE;
        MESSAGE_SIZE_TYPE len;
        RECEIVE_QUEUING_MESSAGE(QUEUING_PORT_ID, 0, (uint8_t *)str,
			&len, &RETURN_CODE);
        if (RETURN_CODE == NO_ERROR) {
            BSP_UARTx_transmit((uint8_t *)str, len);
        }

        HAL_Delay(10);
    }
}
