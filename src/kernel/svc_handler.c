#include "context.h"

void SVC_Handler(void)
{
    ARM_HW_context_state* stack;

    __ASM volatile (
        "AND	R1, LR, #0x0D	\n\t"		// Logical AND with 0xD
		"CMP	R1, #0x0D		\n\t"		// Use CMP to set EQ/NE flag
        "MRSEQ  %0, PSP         \n\t"       // Move PSP into pointer if EQ flag is set
        "MRSNE  %0, MSP         \n\t"       // Move MSP into pointer if NE flag is set
        : "=r"  (stack)
        : :
    ) 

    switch (stack->R0)
    {
        case 0x0000000a:
            {
                //Insert call to CREATE_PROCESS handler here!
            }
        default:
            break;
    }
}