#include <stm32f4xx_hal.h>
#include <apex_types.h>

#include "context.h"
#include "drivers/time_get.h"

void SVC_Handler(void)
{
    ARM_HW_context_state* stack;

    __ASM volatile (
        "TST	LR, #0x4    	\n\t"		// Test bit 2 of EXC_RETURN
		"ITE	EQ      		\n\t"		// Which stack pointer was used?
        "MRSEQ  %0, MSP         \n\t"       // Move MSP into pointer if EQ flag is set
        "MRSNE  %0, PSP         \n\t"       // Move SSP into pointer if NE flag is set
        : "=r"  (stack)
        : :
    );

    switch (stack->R0)
    {
        case 0xc0ffee0a:        // CREATE_PROCESS
        {
            //Insert call to CREATE_PROCESS handler here!
            return;
        }
        case 0xc0ffee0b:        // GET_TIME
        {
            uint64_t time = TIME_Get_Total();
            stack->R0 = NO_ERROR;
            stack->R1 = (uint32_t) time;
            stack->R2 = (uint32_t) (time >> 32);
            return;
        }
        default:
            break;
    }
}