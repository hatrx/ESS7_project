#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "rtc.h"

RTC_HandleTypeDef RTCHandle;
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCdate;


void HAL_RTC_MspInit(RTC_HandleTypeDef *RTCHandle)
{
	(void)RTCHandle;
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_ENABLE();
}
//write brief
void BSP_RTC_init()
{
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

	RTCtime.Hours = 12;
	RTCtime.Minutes = 15;
	RTCtime.Seconds = 00;
	RTCtime.SubSeconds = 10;
	RTCtime.TimeFormat = RTC_HOURFORMAT_24;
	RTCtime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTCtime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&RTCHandle, &RTCtime, FORMAT_BIN);

	RTCdate.WeekDay = RTC_WEEKDAY_MONDAY;
	RTCdate.Month = RTC_MONTH_NOVEMBER;
	RTCdate.Date = 21;
	RTCdate.Year = 16;
	HAL_RTC_SetDate(&RTCHandle, &RTCdate, FORMAT_BIN);
	
}

void get_datetime(struct date* now)
{
	HAL_RTC_GetTime(&RTCHandle, &RTCtime, FORMAT_BIN);
	HAL_RTC_GetDate(&RTCHandle, &RTCdate, FORMAT_BIN);
	now->seconds = RTCtime.Seconds;
	now->minutes = RTCtime.Minutes;
	now->hours = RTCtime.Hours;
	

	/*DateTime now;
	now.subseconds = RTCtime.SubSeconds;
	now.seconds = RTCtime.Seconds;
	now.minutes = RTCtime.Minutes;
	now.hours = RTCtime.Hours;
	now.date = RTCdate.Date;
	now.month = RTCdate.Month;
	now.year = RTCdate.Year;
	return now;*/
}
/*
void print_datetime()
{
	printf("test");
}*/

int RTC_state()
{
	return HAL_RTC_GetState(&RTCHandle) == HAL_RTC_STATE_READY ? 1 : 0;
}