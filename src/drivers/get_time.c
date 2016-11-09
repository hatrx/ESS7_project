#include <stm32f4xx_hal.h>

#include "get_time.h"

uint32_t start = 0;

	volatile unsigned int *DWT_CYCCNT     = (volatile unsigned int *)0xE0001004; //address of the register
	volatile unsigned int *DWT_CONTROL    = (volatile unsigned int *)0xE0001000; //address of the register
	//32-bit, incrementing (up) cycle counter. When enabled, this
	//counter counts the number of core cycles. Counting is suspended
	//when the core is halted in Debug state. The counter is UNKNOWN
	//on reset.
	volatile unsigned int *SCB_DEMCR         = (volatile unsigned int *)0xE000EDFC; //address of the register

uint32_t start_time_ms()
{
	start = HAL_GetTick();
	return start;
}

uint32_t stop_time_ms()
{
	return HAL_GetTick() - start;
}
/*

//https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/DispForm.aspx?ID=11943&RootFolder=/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Duration%20of%20FLOAT%20operations
//http://stackoverflow.com/questions/32610019/arm-m4-instructions-per-cycle-ipc-counters
*/

void start_time()
{

    *SCB_DEMCR   = *SCB_DEMCR | 0x01000000;
    *DWT_CONTROL = 0;
    *DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter
	*DWT_CYCCNT  = 0; // reset the counter
}


unsigned int stop_time()
{
	unsigned int cycles = getCycles();
    *DWT_CONTROL = *DWT_CONTROL | 0 ; // disable the counter
	return cycles;
}

unsigned int getCycles()
{
    return *DWT_CYCCNT;
}