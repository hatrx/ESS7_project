#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "drivers/time_get.h"
#include <drivers/time_get.h>

#include "arinc/partition.h"
#include "context.h"
#include "scheduler.h"


//#define DISABLE_CONTEXT_SWITCH
#ifndef DISABLE_CONTEXT_SWITCH
/**
 * @brief Handles SysTick interrupts. This should fire once every millisecond.
 * This function also handles context switches.
 * Note: This function is declared as naked to tell GCC not to add the usual padding,
 *       which is disastrous in a context switing function
 * TODO: This function does not handle switches from other exception handlers yet,
 *       and it does not handle context switches from functions that use the FPU
 */
__attribute__((naked)) void SysTick_Handler(void)
{
	uint32_t stackpointer = context_save();
	uint32_t exc_return_value;
	__asm volatile ("MOV %0, LR" : "=r" (exc_return_value));


	static process_t* active_process = NULL;
	if (active_process != NULL)
	{
		// Save the value of the stack pointer for later use.
		active_process->stackpointer = stackpointer;
		active_process->exc_return_value = (exc_return_value & 0xFF);
	}

	/* Get pointer to currently running partition. */
	partition_t *part = scheduler_partitionScheduler();

	active_process = scheduler_processScheduler(part);

	// Increase HAL ticks. This is by the HAL_Delay(int) function, so we need to do this.
	HAL_IncTick();
	TIME_Add_Count();

	// Resote the software context of the new process.
	// TODO: Right now, this only handles switches to userspace (and without setting privilege levels),
	// but it should be modified to be more comprehensive.
	__ASM volatile (
		"LDMFD  %[stack]!, {R4-R11}     \n\t"
		"TST	%[exc], #0x4    		\n\t"		// Test bit 2 of EXC_RETURN
		"ITE	EQ      				\n\t"		// Which stack pointer was used?
		"MSREQ	MSP, %[stack]		 	\n\n"		// Update MSP if EQ is set
		"MSRNE 	PSP, %[stack]			\n\t"		// Update PSP if NE is set
		: [stack] "+r" (active_process->stackpointer) : [exc] "r" (active_process->exc_return_value) :
	);

	__ASM volatile(
		"LDR	R1, =0xFFFFFF00 		\n\t"
		"ADD	R1, R1, %[exc]			\n\t"
		"BX		R1						\n\t"
		: : [exc] "r" (active_process->exc_return_value) :
	);
}
#else
void SysTick_Handler(void)
{
	HAL_IncTick();
	TIME_Add_Count();
}
#endif


void context_setup(void (*foo)(void), void *addr) {
	ARM_context_state *stack = (ARM_context_state *)addr;
	ARM_HW_context_state *hw_stack = &stack->hw_stack;
	hw_stack->R0 = 0;
	hw_stack->R1 = 0;
	hw_stack->R2 = 0;
	hw_stack->R3 = 0;
	hw_stack->R12 = 0;
	hw_stack->LR = 0;
	hw_stack->PC = (uint32_t)foo;
	hw_stack->PSR = 0x21000000;
	ARM_SW_context_state *sw_stack  =  &stack->sw_stack;
	sw_stack->R4 = 0;
	sw_stack->R5 = 0;
	sw_stack->R6 = 0;
	sw_stack->R7 = 0;
	sw_stack->R8 = 0;
	sw_stack->R9 = 0;
	sw_stack->R10 = 0;
	sw_stack->R11 = 0;
}
