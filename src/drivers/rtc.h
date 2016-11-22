#ifndef RTC_H
#define RTC_H

struct date
{
	uint8_t subseconds;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t year;
};

#define HOUR ((uint8_t) 12)
#define MINUTE ((uint8_t) 30)
#define SECOND ((uint8_t) 0)
#define SUBSECOND ((uint8_t) 0)

#define DATE ((uint8_t) 21)
#define YEAR ((uint8_t) 16)

#define PRESCALER1 ((uint32_t) 127)
#define PRESCALER2 ((uint32_t) 257)
//The asynchronous prescaler division factor is set to 128, and the synchronous division
	//factor to 256, to obtain an internal clock frequency of 1 Hz (ck_spre) with an LSE frequency
	//of 32.768 kHz.
void HAL_RTC_MspInit(RTC_HandleTypeDef *RTCHandle);
void BSP_RTC_init();

void get_datetime(struct date*);

int RTC_state();

#endif /* UART_H */