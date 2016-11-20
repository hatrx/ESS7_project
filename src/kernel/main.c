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
//#include "drivers/rtc.h"

#include "kernel/context.h"

void dummy1_main(void);
void dummy2_main(void);

RTC_HandleTypeDef RTCHandle;
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCdate;

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

int main(void)
{
	set_system_clock_168mhz();

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	init_onboard_LED(red_led);
	init_onboard_LED(yellow_led);

	//BSP_IWDG_init(3000);		//time slot for the watchdog to be refreshed in (in miliseconds)
	//BSP_RTC_init();

/*
	RCC_OscInitTypeDef        RCC_OscInitStruct;

RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    while(1){}
  }
 //RCC_LSE_Config();
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1){}
  }
*/

	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	//__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	
	__HAL_RCC_RTC_ENABLE();

	RTCHandle.Instance = RTC;
	
	RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RTCHandle.Init.AsynchPrediv = 127;
	RTCHandle.Init.SynchPrediv = 257;
	//The asynchronous prescaler division factor is set to 128, and the synchronous division
//factor to 256, to obtain an internal clock frequency of 1 Hz (ck_spre) with an LSE frequency
//of 32.768 kHz.
	RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	
	HAL_RTC_Init(&RTCHandle);

	RTCtime.Hours = 0x0C;
	RTCtime.Minutes = 0x00;
	RTCtime.Seconds = 0x00;
	RTCtime.SubSeconds = 0x00;
	RTCtime.TimeFormat = RTC_HOURFORMAT_24;
	RTCtime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTCtime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&RTCHandle, &RTCtime, FORMAT_BCD);

	RTCdate.WeekDay = RTC_WEEKDAY_MONDAY;
	RTCdate.Month = RTC_MONTH_NOVEMBER;
	RTCdate.Date = 0x15;
	RTCdate.Year = 0x10;
	HAL_RTC_SetDate(&RTCHandle, &RTCdate, FORMAT_BIN);

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
		HAL_Delay(1000);
		

		HAL_RTC_GetTime(&RTCHandle, &RTCtime, FORMAT_BIN);
		HAL_RTC_GetDate(&RTCHandle, &RTCdate, FORMAT_BIN);
		printf("%" PRIu8 ":%" PRIu8 ":%" PRIu8 " \n", RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
		HAL_RTC_GetState(&RTCHandle);
	}
}
