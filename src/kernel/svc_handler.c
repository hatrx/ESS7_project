#include <stm32f4xx_hal.h>
#include <apex_types.h>

#include "context.h"
#include "drivers/time_get.h"

#include "queuing_port.h"


static uint32_t pop(uint32_t *p)
{
	const uint32_t ret = *(uint32_t *)(*p);
	*p += 4;
	return ret;
}


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

    /* Disable sysTick */
    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_ENABLE_Msk;

    switch (stack->R0) {
        case 0xc0ffee0a:        // CREATE_PROCESS
        {
            //Insert call to CREATE_PROCESS handler here!
            break;
        }
        case 0xc0ffee0b:        // GET_TIME
        {
            uint64_t time = TIME_Get_Total();
            stack->R0 = NO_ERROR;
            stack->R1 = (uint32_t) time;
            stack->R2 = (uint32_t) (time >> 32);
            break;
        }
        case 0xc0ffee0c:        // CREATE_QUEUING_PORT
        {
			uint8_t argc = 7;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            create_queuing_port(
				(char *) argv[0],
				(MESSAGE_SIZE_TYPE) argv[1],
				(MESSAGE_RANGE_TYPE) argv[2],
				(PORT_DIRECTION_TYPE) argv[3],
				(QUEUING_DISCIPLINE_TYPE) argv[4],
				(QUEUING_PORT_ID_TYPE *) argv[5],
				(RETURN_CODE_TYPE *) argv[6]);
            break;
        }
        default:
        break;
    }

    /* Enable sysTick */
    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk |
        SysTick_CTRL_ENABLE_Msk;
}
