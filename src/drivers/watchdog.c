#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "watchdog.h"

//diference between watchdogs: http://electronics.stackexchange.com/questions/123080/independent-watchdog-iwdg-or-window-watchdog-wwdg
//the more reliable one is the independent watchdog

IWDG_HandleTypeDef IwdgHandle;

//This function sets the value from where the timer start counting down
void reload_time_setup()
{
    if(WATCHDOG_TIME_INTERVAL < 4000)				//less than 4 seconds, 1 ms steps
    {
        IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;		//1 tick every milisecond
        IwdgHandle.Init.Reload    = WATCHDOG_TIME_INTERVAL;	//Min_Data = 0 and Max_Data = 0x0FFF
    }
    else if(WATCHDOG_TIME_INTERVAL <= 30000) 	   //less than 30 seconds, 8 ms steps
    {
        IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;					//1 tick every 8 miliseconds
        IwdgHandle.Init.Reload    = (uint32_t)WATCHDOG_TIME_INTERVAL/8;	//Min_Data = 0 and Max_Data = 0x0FFF
    }
}

//This function sets the peripherals needed by the watchdog. There are none required in this case
void HAL_IWDG_MspInit(IWDG_HandleTypeDef *hiwdg)
{
    (void) hiwdg;
}

//Thsi function initialises the watchdog
int BSP_IWDG_Init()
{
    IwdgHandle.Instance = IWDG;
    reload_time_setup();

    if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)		//Enables the watchdog and configures he prescaler
    {
        return -1;
    }

    if(HAL_IWDG_Start(&IwdgHandle) != HAL_OK)		//Starts the watchdog
    {
        return -1;
    }

    return HAL_IWDG_GetState(&IwdgHandle) == HAL_IWDG_STATE_READY ? 0 : -1;
}

//This functions refreshes the watchdog by reloading its value
void IWDG_Refresh()
{
    HAL_IWDG_Refresh(&IwdgHandle);
}

//Get the state of the watchdog
int IWDG_State()
{
    return HAL_IWDG_GetState(&IwdgHandle) == HAL_IWDG_STATE_READY ? 0 : -1;
}
