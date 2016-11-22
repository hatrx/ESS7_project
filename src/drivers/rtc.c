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
	__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);		//without this line we have unpredictable behaviour

	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_ENABLE();
}
//write brief
void BSP_RTC_init()
{
	RTCHandle.Instance 			= RTC;
	RTCHandle.Init.HourFormat 	= RTC_HOURFORMAT_24;
	RTCHandle.Init.AsynchPrediv = PRESCALER1;
	RTCHandle.Init.SynchPrediv 	= PRESCALER2;
	RTCHandle.Init.OutPut 		= RTC_OUTPUT_DISABLE;
	RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTCHandle.Init.OutPutType 	= RTC_OUTPUT_TYPE_OPENDRAIN;

	HAL_RTC_Init(&RTCHandle);

	RTCtime.Hours 				= HOUR;
	RTCtime.Minutes				= MINUTE;
	RTCtime.Seconds 			= SECOND;
	RTCtime.SubSeconds 			= SUBSECOND;
	RTCtime.TimeFormat 			= RTC_HOURFORMAT_24;
	RTCtime.DayLightSaving 		= RTC_DAYLIGHTSAVING_NONE;
	RTCtime.StoreOperation 		= RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&RTCHandle, &RTCtime, FORMAT_BIN);

	RTCdate.WeekDay 	= RTC_WEEKDAY_MONDAY;
	RTCdate.Month 		= RTC_MONTH_NOVEMBER;
	RTCdate.Date 		= DATE;
	RTCdate.Year 		= YEAR;
	HAL_RTC_SetDate(&RTCHandle, &RTCdate, FORMAT_BIN);
	
}

void get_datetime(struct date* now)
{
	HAL_RTC_GetTime(&RTCHandle, &RTCtime, FORMAT_BIN);
	HAL_RTC_GetDate(&RTCHandle, &RTCdate, FORMAT_BIN);
	now->subseconds = RTCtime.SubSeconds;
	now->seconds 	= RTCtime.Seconds;
	now->minutes 	= RTCtime.Minutes;
	now->hours 		= RTCtime.Hours;
	now->date 		= RTCdate.Date;
	now->month		= RTCdate.Month;
	now->year 		= RTCdate.Year;
}

int RTC_state()
{
	return HAL_RTC_GetState(&RTCHandle) == HAL_RTC_STATE_READY ? 1 : 0;
}