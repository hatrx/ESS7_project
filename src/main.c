#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "uart.h"
#include "kernel/context.h"
#include "partitions/dummy1/dummy1.h"
#include "partitions/dummy2/dummy2.h"


volatile void* taskStacks[3];
volatile uint8_t activeProcess = 0;


/**
 * @brief Handles SysTick interrupts. This should fire once every millisecond.
 * This function also handles context switches.
 * Note: This function is declared as naked to tell GCC not to add the usual padding, which is disastrous in a context switing function
 * TODO: This function does not handle switches from other exception handlers yet, and it does not handle context switches from functions that use the FPU
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
	register volatile void * stackpointer __asm ("r1");

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

	if (activeProcess == 1)
		activeProcess = 2;
	else
		activeProcess = 1;

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

void UsageFault_Handler(void)
{
	printf("UsageFault_Handler\n");
}

void HardFault_Handler(void)
{
	printf("HardFault_Handler\n");
	// SCB_Type *something = SCB;
	// uint32_t hfsr = something->HFSR;
	// uint32_t cfsr = something->CFSR;
	while(1);
}

void MemManager_Handler(void)
{
	printf("MemManager_Handler\n");
}

void SVC_Handler(void)
{
	printf("SVC_Handler\n");
}

void set_system_clock_168mhz(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	const uint32_t HSE_mhz = HSE_VALUE/10e5;
	const bool HSE_is_odd  = (HSE_mhz & 1) == 1;

	RCC_OscInitTypeDef RCC_OscInit = {
		.OscillatorType = RCC_OSCILLATORTYPE_HSE,
		.HSEState       = RCC_HSE_ON,
		.PLL = {
			.PLLState   = RCC_PLL_ON,
			.PLLSource  = RCC_PLLSOURCE_HSE,
			.PLLM       = HSE_is_odd ? HSE_mhz : (HSE_mhz / 2), // should match HSE freq in mhz so HSE/PLLM == 1
			.PLLN       = HSE_is_odd ? (168*2) : 168, // 336 / 2 == 168 == max core clk
			.PLLP       = RCC_PLLP_DIV2, // ((HSE/pllm) * plln) / pllp == 168 (max core clk)
			.PLLQ       = 7, // ((HSE/pllm) * plln) / pllq == 48 (USB needs 48 and sdio needs 48 or lower)
		},
	};

	if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK) {
		while (1);
	}

	RCC_ClkInitTypeDef RCC_ClkInit = {
		.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
		.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider  = RCC_SYSCLK_DIV1,
		.APB1CLKDivider = RCC_HCLK_DIV4,
		.APB2CLKDivider = RCC_HCLK_DIV2,
	};
	// We set FLASH_LATENCY_5 as we are in vcc range 2.7-3.6 at 168mhz
	// see datasheet table 10 at page 80.
	if (HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_5) != HAL_OK) {
		while (1);
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported */
	if (HAL_GetREVID() == 0x1001) {
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}


int main(void)
{
	UART_HandleTypeDef UartHandle;

	void (*main_function);
	main_function = &dummy1_main;

	ARM_context_state *stack = (ARM_context_state *) 0x20001000;
	ARM_HW_context_state *hw_stack = &stack->hw_stack;
	hw_stack->R0 = 0;
	hw_stack->R1 = 0;
	hw_stack->R2 = 0;
	hw_stack->R3 = 0;
	hw_stack->R12 = 0;
	hw_stack->LR = 0;
	hw_stack->PC = (uint32_t)main_function;
	hw_stack->PSR = 0x21000000;
	ARM_SW_context_state * sw_stack  =  &stack->sw_stack;
	sw_stack->R4 = 0;
	sw_stack->R5 = 0;
	sw_stack->R6 = 0;
	sw_stack->R7 = 0;
	sw_stack->R8 = 0;
	sw_stack->R9 = 0;
	sw_stack->R10 = 0;
	sw_stack->R11 = 0;

	taskStacks[1] = (void *) stack;


	main_function = &dummy2_main;

	ARM_context_state *stack2 = (ARM_context_state *) 0x20003000;
	ARM_HW_context_state *hw_stack2 = &stack2->hw_stack;
	hw_stack2->R0 = 0;
	hw_stack2->R1 = 0;
	hw_stack2->R2 = 0;
	hw_stack2->R3 = 0;
	hw_stack2->R12 = 0;
	hw_stack2->LR = 0;
	hw_stack2->PC = (uint32_t)main_function;
	hw_stack2->PSR = 0x21000000;
	ARM_SW_context_state * sw_stack2 = &stack2->sw_stack;
	sw_stack2->R4 = 0;
	sw_stack2->R5 = 0;
	sw_stack2->R6 = 0;
	sw_stack2->R7 = 0;
	sw_stack2->R8 = 0;
	sw_stack2->R9 = 0;
	sw_stack2->R10 = 0;
	sw_stack2->R11 = 0;

	taskStacks[2] = (void *) stack2;

	HAL_Init();
	set_system_clock_168mhz();
	HAL_UART_MspInit(&UartHandle);

	/*Configure GPIO pin : PC13 The red LED on the board*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	while (1);
}
