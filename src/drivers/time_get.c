#include <stm32f4xx_hal.h>

#include "time_get.h"

uint16_t start = 0;
int64_t total = 0;

//Registers from teh ARM architecture manual in the Debug Watchpoint and Trace section
volatile unsigned int *DWT_CYCCNT     = (unsigned int *)0xE0001004; //address of the register DWT_CYCCNT - counter
volatile unsigned int *DWT_CONTROL    = (unsigned int *)0xE0001000; //address of the register DWT_CTRL - DWT control
volatile unsigned int *SCB_DEMCR      = (unsigned int *)0xE000EDFC; //address of the register used for debugging


//These functions compute a basic time difference using the system tick
uint16_t TIME_Start_ms()
{
    start = HAL_GetTick();
    return start;
}

//The return value is in miliseconds
uint16_t TIME_Stop_ms()
{
    return HAL_GetTick() - start;
}


/*
//https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/DispForm.aspx?ID=11943&RootFolder=/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Duration%20of%20FLOAT%20operations
//http://stackoverflow.com/questions/32610019/arm-m4-instructions-per-cycle-ipc-counters
*/

int64_t convert_cycles_to_time(uint32_t cycles)
{
    return (int64_t)((cycles / 168) * 1000);		//multiplies the cycles with 5,95 nanoseconds to get the time
    //TODO: Use HAL library function to get CPU freq instead of using hardcoded value.
}

//This function initialises the registers needed to get the system tick counter running
void TIME_Start_ns()
{
    *SCB_DEMCR   = *SCB_DEMCR | 0x01000000;		//maybe needed for debugging support
    *DWT_CONTROL = 0;							//clears the register (Debug Watchpoint and Trace)
    *DWT_CONTROL = *DWT_CONTROL | 1 ;			//enable the counter (CYCCNT is enabled by the last bit)
    *DWT_CYCCNT = 0;						//reset the counter
}



//32-bit, incrementing (up) cycle counter. When enabled, this
//counter counts the number of core cycles. Counting is suspended
//when the core is halted in Debug state. The counter is UNKNOWN
//on reset.

int64_t TIME_Stop_ns()
{
    int64_t time = convert_cycles_to_time(*DWT_CYCCNT);
    *DWT_CONTROL = *DWT_CONTROL | 0 ; // disable the counter
    return time;
}

void TIME_Add_Count()
{
    total = total + TIME_Stop_ns();
    *DWT_CYCCNT = 0;
}

int64_t TIME_Get_Total()
{
    return total;
}

//The instant value of the core cycles counter
uint32_t TIME_Get_Cycles()
{
    return *DWT_CYCCNT;
}
