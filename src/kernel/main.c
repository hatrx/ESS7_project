#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/get_time.h"
#include "drivers/rtc.h"

#include "kernel/context.h"

void dummy1_main(void);
void dummy2_main(void);

void UsageFault_Handler(void)
{
	printf("UsageFault_Handler\n");
}


void HardFault_Handler(void)
{
	printf("HardFault_Handler\n");
	// SCB_Type *something = SCB;
	// uint32_t hfsr = something->HFSR;
	// uint32_t cfsr = something->CFSR;
	while(1);
}


void MemManager_Handler(void)
{
	printf("MemManager_Handler\n");
}


void SVC_Handler(void)
{
	printf("SVC_Handler\n");
}


void Error_Handler(void)
{
	printf("Initialisation Error\n");
	while(1);
}


int main(void)
{
	set_system_clock_168mhz();

	if (BSP_UART_Init() != 0)
	{
		Error_Handler();		// Shit not working!
	}

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

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
	//init_mpu(0x20000000 + 0x2000, MPU_1KB);

	setup_contexts(&dummy1_main, (void *)0x20001000);
	setup_contexts(&dummy2_main, (void *)0x20003000);

	printf("\n");
	printf("dummy1: %02X\n", &dummy1_main);
	printf("dummy2: %02X\n", &dummy2_main);


	HAL_Init();

	//uint32_t counter = 0;

	while (1) {
		//printf("Hello world! : %"PRIu32"\n", counter++);

		TIME_Start_ns();
		HAL_Delay(1000);
		uint64_t x = TIME_Stop_ns();
		//printf("%" PRIu64 " -->\n", x);		//printf won't work on 64 bit values
/*
		Date_Time_t printdate;
		RTC_Get_Date_Time(&printdate);
		IWDG_Refresh();
		printf("%02" PRIu8 ":%02" PRIu8 ":%02" PRIu8 " ", printdate.hours, printdate.minutes, printdate.seconds);
		printf("%02" PRIu8 "-%02" PRIu8 "-%02" PRIu8 " \n", printdate.date, printdate.month, 2000 + printdate.year);
*/
	}
}
