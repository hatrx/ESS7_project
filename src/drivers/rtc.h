#ifndef RTC_H
#define RTC_H

struct date
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
};


void HAL_RTC_MspInit(RTC_HandleTypeDef *RTCHandle);
void BSP_RTC_init();

void get_datetime(struct date*);
//void print_datetime();
int RTC_state();

#endif /* UART_H */