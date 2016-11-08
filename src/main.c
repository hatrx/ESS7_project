#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "uart.h"
#include "kernel/context.h"
#include "partitions/dummy1/dummy1.h"

volatile void* taskStacks[2];
//volatile void* currProcess = NULL;
volatile uint8_t activeProcess = 0;

__attribute__((naked)) void SysTick_Handler(void)
{
	//static volatile uint32_t msCount = 0;
	register volatile uint32_t wasUserspace __asm ("r0");
	//__ASM volatile ("MOV R0, LR");
	//wasUserspace &= 0xF;
	register volatile void * stackpointer __asm ("r1");
	__ASM volatile (
		"AND	%0, LR, #0x0D	\n\t"		// Logical AND with 0xD
		"CMP	%0, #0xD		\n\t"		// Use CMP to set EQ/NE flag
		"BEQ 	use_psp			\n\t"		// Branch to use_psp if EQ is set
		"BNE	use_msp			\n\n"		// Branch to use_msp  if NE is set
		"use_psp:				\n\t"
		"MRS    %1, PSP			\n\t"
        "STMFD  %1!, {R4-R11}   \n\t"
        "MSR    PSP, %1         \n\t"
		"B 		exit			\n\n"
		"use_msp:				\n\t"
		"MRS    %1, MSP			\n\t"
        "STMFD  %1!, {R4-R11}   \n\t"
        "MSR    MSP, %1         \n\n"
		"exit:					\n\t"
		: "+r" (wasUserspace), "=r" (stackpointer)
		: :
	);
	taskStacks[activeProcess] = stackpointer;
	HAL_IncTick();
	/*msCount++;

	ARM_context_state * state_kernel = (ARM_context_state *) taskStacks[0];
	ARM_context_state * state_user = (ARM_context_state *) taskStacks[1];

	/*__ASM volatile (
		"MOV %0, LR		\n"
		: "=r" (LR)
	);*/

	activeProcess = 1;
	//uint32_t tmp = CONTEXT_restoreContext((uint32_t) taskStacks[activeProcess]);
	//register volatile uint32_t tmp = 0;
	__ASM volatile (
        "LDMFD  %0!, {R4-R11}       \n"
        //"MOV R1, %0                 \n"
		"MSR 	PSP, %0				\n"
        : "+r" ((uint32_t) taskStacks[activeProcess]) : 
    );
	//tmp = (uint32_t) taskStacks[activeProcess];
	//__set_PSP(tmp);*/
	__ASM volatile (
		"LDR PC,=0xFFFFFFFD"
	);
}

void UsageFault_Handler(void)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void HardFault_Handler(void)
{
	SCB_Type * something = SCB;
	uint32_t hfsr = something->HFSR;
	uint32_t cfsr = something->CFSR;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
	while(1);
}

void MemManager_Handler(void)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void SVC_Handler(void)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
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

	ARM_context_state * stack = (ARM_context_state *) 0x20010000;
	ARM_HW_context_state * hw_stack = &stack->hw_stack;
	hw_stack->R0=0;
	hw_stack->R1=0;
	hw_stack->R2=0;
	hw_stack->R3=0;
	hw_stack->R12=0;
	hw_stack->LR=0;
	hw_stack->PC=main_function;
	hw_stack->PSR=0x21000000;
	ARM_SW_context_state * sw_stack = &stack->sw_stack;
	sw_stack->R4=0;
	sw_stack->R5=0;
	sw_stack->R6=0;
	sw_stack->R7=0;
	sw_stack->R8=0;
	sw_stack->R9=0;
	sw_stack->R10=0;
	sw_stack->R11=0;

	taskStacks[1] = (void *) stack;

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

	/*__ASM volatile (
		"MRS    %0, MSP"
		: "=r" (currProcess)
	);*/

	while (1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
		HAL_Delay(1000);
	}
}
