#include <stm32f4xx_hal.h>
#include <apex_types.h>
#include <apex_queuing.h>

#include "context.h"
#include "drivers/time_get.h"
#include "arinc/process.h"

#include "arinc/queuing_port.h"


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
			uint8_t argc = 3;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            runtime_create_process(
				(PROCESS_ATTRIBUTE_TYPE *) argv[0],
				(PROCESS_ID_TYPE *) argv[1],
				(RETURN_CODE_TYPE *) argv[2]);
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
		case 0xc0ffee0d:        // CREATE_QUEUING_PORT
        {
			uint8_t argc = 5;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            recieve_queuing_message(
				(QUEUING_PORT_ID_TYPE) argv[0],
				(SYSTEM_TIME_TYPE) argv[1],
				(MESSAGE_ADDR_TYPE) argv[2],
				(MESSAGE_SIZE_TYPE *) argv[3],
				(RETURN_CODE_TYPE *) argv[4]);
            break;
        }
		case 0xc0ffee0e:        // CREATE_QUEUING_PORT
        {
			uint8_t argc = 5;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            send_queuing_message(
				(QUEUING_PORT_ID_TYPE) argv[0],
				(MESSAGE_ADDR_TYPE) argv[1],
				(MESSAGE_SIZE_TYPE) argv[2],
				(SYSTEM_TIME_TYPE) argv[3],
				(RETURN_CODE_TYPE *) argv[4]);
            break;
        }
		case 0xc0ffee0f:        // CREATE_QUEUING_PORT
        {
			uint8_t argc = 3;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            get_queuing_port_id(
				(char *) argv[0],
				(QUEUING_PORT_ID_TYPE *) argv[1],
				(RETURN_CODE_TYPE *) argv[2]);
            break;
        }
		case 0xc0ffee10:        // CREATE_QUEUING_PORT
        {
			uint8_t argc = 3;
			uint32_t argv[argc];

			while(argc--) {
				argv[argc] = pop(&stack->R2);
			}

            get_queuing_port_status(
				(QUEUING_PORT_ID_TYPE) argv[0],
				(QUEUING_PORT_STATUS_TYPE *) argv[1],
				(RETURN_CODE_TYPE *) argv[2]);
            break;
        }
		case 0xc0ffee11:        // CREATE_QUEUING_PORT
        {
            process_stop_self();
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
