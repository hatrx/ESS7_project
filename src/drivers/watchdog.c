#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "watchdog.h"

//diference between watchdogs: http://electronics.stackexchange.com/questions/123080/independent-watchdog-iwdg-or-window-watchdog-wwdg
//the more reliable one is the independent watchdog

IWDG_HandleTypeDef IwdgHandle;

void reload_time_setup(uint16_t miliseconds)
{
	if(miliseconds < 4000)				//less than 4 seconds, 1 ms steps
	{
		printf("%d\n", miliseconds);
		IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
		IwdgHandle.Init.Reload    = miliseconds;
	}
	else if(miliseconds <= 30000) 	   //less than 30 seconds, 8 ms steps
	{
		printf("%d\n", miliseconds);
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
	//calls MspInit by itself
	reload_time_setup(miliseconds);           //sets the prescaler and reload values
	if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
	{
		//Initialisation error
	}
	if(HAL_IWDG_Start(&IwdgHandle) != HAL_OK)
	{
		//Start error
	}
}

void BSP_IWDG_refresh()
{
	HAL_IWDG_Refresh(&IwdgHandle);
}