#ifndef RTC_H
#define RTC_H


#include <stdint.h>
#include <stm32f4xx_hal_rtc.h>

typedef struct
{
	uint8_t subseconds;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} Date_Time_t;

//Date and Time defined at compile time
#define HOUR ((uint8_t) 9)
#define MINUTE ((uint8_t) 45)
#define SECOND ((uint8_t) 0)
#define SUBSECOND ((uint8_t) 0)

#define DATE ((uint8_t) 23)
#define YEAR ((uint8_t) 16)

/*
The asynchronous prescaler division and the synchronous division should divide our frequency,
so our result would be 1 Hz. This way we have the time base of 1 second.
The registers receive the values of 128 and 256. TIME_BASE = CLOCK FREQ./PRESCALER1 * PRESCALER2
TIME_BASE = 32768 / 128 * 256
*/
#define PRESCALER1 ((uint32_t) 127)		//Min_Data = 0x00 and Max_Data = 0x7F (127)
#define PRESCALER2 ((uint32_t) 255)		//Min_Data = 0x00 and Max_Data = 0x7FFF (32767)

void HAL_RTC_MspInit(RTC_HandleTypeDef *RTCHandle);
int BSP_RTC_Init();

void RTC_Get_Date_Time(Date_Time_t*);

int RTC_State();

#endif /* RTC_H */
