#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stm32f4xx_hal.h>

void reload_time_setup(uint16_t miliseconds);
void HAL_IWDG_MspInit(IWDG_HandleTypeDef *hiwdg);
void BSP_IWDG_init(uint16_t miliseconds);
void BSP_IWDG_refresh();

#endif
