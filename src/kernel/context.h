#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

typedef struct {
	uint32_t R0;    //General purpose R0 Register
	uint32_t R1;    //General purpose R1 Register
	uint32_t R2;    //General purpose R2 Register
	uint32_t R3;    //General purpose R3 Register
	uint32_t R12;   //General purpose R12 Register
	uint32_t LR;    //Link Register
	uint32_t PC;    //Program Counter Register
	uint32_t PSR;   //Program Status Register
} ARM_HW_context_state;

typedef struct {
	uint32_t R4;    //General purpose R4 Register
	uint32_t R5;    //General purpose R5 Register
	uint32_t R6;    //General purpose R6 Register
	uint32_t R7;    //General purpose R7 Register
	uint32_t R8;    //General purpose R8 Register
	uint32_t R9;    //General purpose R9 Register
	uint32_t R10;   //General purpose R10 Register
	uint32_t R11;   //General purpose R11 Register
} ARM_SW_context_state;

typedef struct {
	ARM_SW_context_state sw_stack;
	ARM_HW_context_state hw_stack;
} ARM_context_state;

/**
 * @brief Save the content of registers R4 to R11 to memory
 * @return Returns the value of the stack pointer after saving the context
 */
__attribute__((always_inline))
static inline uint32_t context_save()
{
	// Create a new variable to store the context-saved-stack pointer of the previous process.
	// We explicitly choose the register to avoid GCC picking a register that hasn't been saved yet.
	register uint32_t stackpointer __asm("r0");
	// Context switch logic.
	// We only save R4-R11 on the stack, because the NVIC already saved the other registers before calling this function.
	__asm volatile (
		"AND	R1, LR, #0x0D	\n\t"		// Logical AND with 0xD
		"CMP	R1, #0x0D		\n\t"		// Use CMP to set EQ/NE flag
		"BEQ 	use_psp			\n\t"		// Branch to use_psp if EQ is set
		"BNE	use_msp		 	\n\n"		// Branch to use_msp  if NE is set
		"use_psp:				\n\t"
		"MRS    %0, PSP			\n\t"		// Move Process Stack Pointer to R1 (the stackpointer variable)
		"STMFD  %0!, {R4-R11}   \n\t"		// Store registers R4 to R11 on the stack. (The FD in STMFB means fully descending [stack])
		"MSR    PSP, %0         \n\t"		// Opdate the actual PSP stackpointer register with the new value after STMFD.
		"B 		exit			\n\n"		// Skip over the use_msp routine
		"use_msp:				\n\t"
		"MRS    %0, MSP			\n\t"		// Move Master Stack Pointer to R1 (the stackpointer variable)
		"STMFD  %0!, {R4-R11}   \n\t"		// Store registers R4 to R11 on the stack.
		"MSR    MSP, %0         \n\n"		// Opdate the actual MSP stackpointer register with the new value after STMFD.
		"exit:					\n\t"
		: "=r" (stackpointer)
		: :
	);
	return stackpointer;
}


void context_setup(void (*foo)(void), void *addr);


#endif /* CONTEXT_H */
