#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "watchdog.h"

//diference between watchdogs: http://electronics.stackexchange.com/questions/123080/independent-watchdog-iwdg-or-window-watchdog-wwdg
//the more reliable one is the independent watchdog

IWDG_HandleTypeDef IwdgHandle;

void reload_time_setup(uint16_t miliseconds)
{
    if(miliseconds < 4000)
    {
        IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
        IwdgHandle.Init.Reload    = miliseconds;
    }
    else if(miliseconds <= 30000)
    {
        IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
        IwdgHandle.Init.Reload    = (int)miliseconds/8;
    }
}

void HAL_IWDG_MspInit(IWDG_HandleTypeDef *hiwdg)
{
    (void) hiwdg;
    IwdgHandle.Instance = IWDG;
    
}

void BSP_IWDG_init(uint16_t miliseconds)
{
    HAL_IWDG_MspInit(&IwdgHandle);
    reload_time_setup(miliseconds);
    if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
        //printf("Initialisation error\n");
    }
    if(HAL_IWDG_Start(&IwdgHandle) != HAL_OK)
    {
       // printf("Start error\n");
    } 
}

void BSP_IWDG_refresh()
{
    HAL_IWDG_Refresh(&IwdgHandle);
}