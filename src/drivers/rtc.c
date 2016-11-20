#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "rtc.h"

RTC_HandleTypeDef RTCHandle;
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCdate;


void HAL_RTC_MspInit(RTC_HandleTypeDef *RTCHandle)
{
	(void)RTCHandle;

}
//write brief
void BSP_RTC_init()
{
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_ENABLE();

	RTCHandle.Instance = RTC;
	
	RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RTCHandle.Init.AsynchPrediv = 124;
	RTCHandle.Init.SynchPrediv = 7999;
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
	HAL_RTC_SetDate(&RTCHandle, &RTCdate, FORMAT_BCD);

}