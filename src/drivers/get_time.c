#include <stm32f4xx_hal.h>

#include "get_time.h"

uint16_t start = 0;

volatile unsigned int *DWT_CYCCNT     = (unsigned int *)0xE0001004; //address of the register DWT_CYCCNT
volatile unsigned int *DWT_CONTROL    = (unsigned int *)0xE0001000; //address of the register DWT_CTRL
volatile unsigned int *SCB_DEMCR      = (unsigned int *)0xE000EDFC; //address of the register

//32-bit, incrementing (up) cycle counter. When enabled, this
//counter counts the number of core cycles. Counting is suspended
//when the core is halted in Debug state. The counter is UNKNOWN
//on reset.

uint16_t start_time_ms()
{
	start = HAL_GetTick();
	return start;
}

uint16_t stop_time_ms()
{
	return HAL_GetTick() - start;
}
/*

//https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/DispForm.aspx?ID=11943&RootFolder=/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Duration%20of%20FLOAT%20operations
//http://stackoverflow.com/questions/32610019/arm-m4-instructions-per-cycle-ipc-counters
*/

uint32_t convert_cycles_to_time(uint32_t cycles)	//max 4 seconds
{
	return (cycles / 168.0) * 1000;
}

void start_time()
{
	*SCB_DEMCR   = *SCB_DEMCR | 0x01000000;   //maybe needed for debugging support
	*DWT_CONTROL = 0;
	*DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter
	*DWT_CYCCNT  = 0; // reset the counter
}


uint32_t stop_time()
{
	uint32_t time = convert_cycles_to_time(*DWT_CYCCNT);
	*DWT_CONTROL = *DWT_CONTROL | 0 ; // disable the counter
	return time;
}

uint32_t getCycles()
{
	return *DWT_CYCCNT;
}