#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stm32f4xx_hal.h>

//The time interval can be from 1ms to 30000ms
//Set the interval, and refresh the watchdog before the time runs out
#define WATCHDOG_TIME_INTERVAL ((uint32_t) 5000)

void reload_time_setup();
void HAL_IWDG_MspInit(IWDG_HandleTypeDef *hiwdg);
int BSP_IWDG_init();
void IWDG_refresh();
int IWDG_state();

#endif
