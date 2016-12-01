#include <stdint.h>

#include <drivers/utils.h>
#include <drivers/onboard_leds.h>
#include <kernel/arinc/queuing_port.h>
#include <drivers/uart.h>

#include <apex_queuing.h>


void stdio_sys_main(void)
{
	QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
	RETURN_CODE_TYPE RETURN_CODE;
	//CREATE_QUEUING_PORT("sys_stio", 32, 32, DESTINATION, FIFO, &QUEUING_PORT_ID, &RETURN_CODE);

	char str[1024];
	while(1) {
		RETURN_CODE_TYPE RETURN_CODE;
		MESSAGE_SIZE_TYPE len;
		//SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
		//RECEIVE_QUEUING_MESSAGE(QUEUING_PORT_ID, 0, (uint8_t *)str, &len, &RETURN_CODE);
		//SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		if (RETURN_CODE == NO_ERROR) {
			//BSP_UARTx_transmit((uint8_t *)str, len);
		}

		HAL_Delay(10);
	}
}
