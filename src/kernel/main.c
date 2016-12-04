#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/rtc.h"
#include "drivers/utils.h"
#include "drivers/time_get.h"

#include "kernel/error_handler.h"
#include "kernel/arinc/partition.h"
#include "kernel/arinc/queuing_port.h"


int main(void)
{
    /* Disable sysTick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    set_system_clock_168mhz();

    if (BSP_UART_Init() != 0)
    {
		Error_Handler(); // Shit not working!
    }

    init_onboard_LED(red_led);
    init_onboard_LED(yellow_led);

    // Clear the UART terminal windows (if it supports ANSI Escape codes)
    printf("%c[%dJ", 0x1B, 2);
    printf("%c[0;0H", 0x1B);
    /*
	if (BSP_IWDG_Init() != 0)		//Independent watchdog time initilisation
	{
		Error_Handler();		// Shit not working!
	}

	if (BSP_RTC_Init() != 0)		//Real time clock initialisation. Define the time interval in the header file
	{
		Error_Handler();		// Shit not working!
	}
	*/

	/* Initialize all ports */
    init_queuing_ports();
	init_partitions();
    TIME_Start_ns();

    /* Enable sysTick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    __asm volatile (
		"MRS R4, CONTROL			\n\t"
		"ORR R4, R4, #0x1			\n\t"
		"MSR CONTROL, R4			\n\t"
		: : :
	);

    while (1);
}
