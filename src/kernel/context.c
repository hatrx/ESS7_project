#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "context.h"

//#define DISABLE_CONTEXT_SWITCH
#define MAX_PROCESS	3

volatile void* taskStacks[MAX_PROCESS];
volatile uint8_t activeProcess = 0;
volatile uint8_t n_process = 1;
volatile uint32_t timings1[MAX_PROCESS] = {2000, 2000};
volatile uint32_t timings2 = 0;


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
	//static volatile uint32_t msCount = 0;

	// Create a new variable to store previous EXC_RETURN code.
	// We explicitly choose the destination register to avoid overwriting registers
	// that have not yet been saved by hardware.
	register volatile uint32_t wasUserspace __asm ("r0");
	//__ASM volatile ("MOV R0, LR");
	//wasUserspace &= 0xF;
	// Create a new variable to store the context-saved-stack pointer of the previous process.
	// Again we explicitly choose the register to avoid GCC picking a register that hasn't been saved yet.
	register volatile void *stackpointer __asm ("r1");

	// Context switch logic.
	// We only save R4-R11 on the stack, because the NVIC already saved the other registers before calling this function.
	__ASM volatile (
		"AND	%0, LR, #0x0D	\n\t"		// Logical AND with 0xD
		"CMP	%0, #0xD		\n\t"		// Use CMP to set EQ/NE flag
		"BEQ 	use_psp			\n\t"		// Branch to use_psp if EQ is set
		"BNE	use_msp			\n\n"		// Branch to use_msp  if NE is set
		"use_psp:				\n\t"
		"MRS    %1, PSP			\n\t"		// Move Process Stack Pointer to R1 (the stackpointer variable)
		"STMFD  %1!, {R4-R11}   \n\t"		// Store registers R4 to R11 on the stack. (The FD in STMFB means fully descending [stack])
		"MSR    PSP, %1         \n\t"		// Opdate the actual PSP stackpointer register with the new value after STMFD.
		"B 		exit			\n\n"		// Skip over the use_msp routine
		"use_msp:				\n\t"
		"MRS    %1, MSP			\n\t"		// Move Master Stack Pointer to R1 (the stackpointer variable)
		"STMFD  %1!, {R4-R11}   \n\t"		// Store registers R4 to R11 on the stack.
		"MSR    MSP, %1         \n\n"		// Opdate the actual MSP stackpointer register with the new value after STMFD.
		"exit:					\n\t"
		: "+r" (wasUserspace), "=r" (stackpointer)
		: :
	);

	// Save the value of the stack pointer for later use.
	taskStacks[activeProcess] = stackpointer;

	// Increase HAL ticks. This is by the HAL_Delay(int) function, so we need to do this.
	HAL_IncTick();
	if(timings2 == HAL_GetTick()){
		
		timings2 = 0;
		++activeProcess;
		if (activeProcess == MAX_PROCESS) {
			activeProcess = 1;
		}
		
	}
	else{
		if(timings2 == 0)
			timings2 = HAL_GetTick() + timings1[n_process];		
		
	}

	// Increment the millisecond counter.
	// TODO: Handle the fact that this handler is NOT called every millisecond as the documentatio otherwise shows.
	/*msCount++;

	// Primarily used for debugging. Can easily be deleted.
	ARM_context_state * state_kernel = (ARM_context_state *) taskStacks[0];
	ARM_context_state * state_user = (ARM_context_state *) taskStacks[1];

	// The the index current active process. Right now this is hardcoded to 1, but should later be incremented.
	activeProcess = 1;

	//uint32_t tmp = CONTEXT_restoreContext((uint32_t) taskStacks[activeProcess]);
	//register volatile uint32_t tmp = 0;

	*/

	// Resote the software context of the new process.
	// TODO: Right now, this only handles switches to userspace (and without setting privilege levels),
	// but it should be modified to be more comprehensive.
	__ASM volatile (
		"LDMFD  %0!, {R4-R11}       \n"
		"MSR 	PSP, %0				\n"
		: "+r" ((uint32_t) taskStacks[activeProcess]) :
	);

	// Switch to using the PSP, and end the function.
	// We need to set the PC with an EXC_RETURN value. This value tells the NVIC which mode to return to;
	// see the ARM documentation to see the meaning of these values.
	__ASM volatile (
		"LDR PC,=0xFFFFFFFD"
	);
}
#else
void SysTick_Handler(void)
{
	HAL_IncTick();
}
#endif


int setup_contexts(void (*foo)(void), void *addr) {
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

	if (n_process < MAX_PROCESS) {
		taskStacks[n_process] = (void *)stack;
		++n_process;
		return 1;
	}

	return 0;
}
