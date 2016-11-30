#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "context.h"
#include "scheduler.h"

#include <drivers/time_get.h>

#include "arinc/partition.h"

#include "drivers/time_get.h"

//#define DISABLE_CONTEXT_SWITCH

static process_t* activeProcess = NULL;

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

	if (activeProcess != NULL)
	{
		// Save the value of the stack pointer for later use.
		activeProcess->stackpointer = stackpointer;
		activeProcess->exc_return_value = (exc_return_value & 0xFF);
	}

	// Increase HAL ticks. This is by the HAL_Delay(int) function, so we need to do this.
	HAL_IncTick();
	TIME_Add_Count();
	scheduler_partitionScheduler();
	activeProcess = scheduler_processScheduler();

	// Resote the software context of the new process.
	// TODO: Right now, this only handles switches to userspace (and without setting privilege levels),
	// but it should be modified to be more comprehensive.
	__ASM volatile (
		"LDMFD  %0!, {R4-R11}       \n"
		"MSR 	PSP, %0				\n"
		: "+r" (activeProcess->stackpointer) :
	);

	__ASM volatile (
		"LDR	R1, =0xFFFFFF00 		\n\t"
		"ADD	R1, R1, %0				\n\t"
		"BX		R1						\n\t"
		: : "r" (activeProcess->exc_return_value) :
	);
}
#else
void SysTick_Handler(void)
{
	HAL_IncTick();
	TIME_Add_Count();
}
#endif


int context_setup(void (*foo)(void), void *addr) {
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

	return 0;
}
