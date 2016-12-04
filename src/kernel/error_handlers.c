#include <stm32f4xx_hal.h>

#include <stdio.h>
#include <stdint.h>

#include <drivers/onboard_leds.h>

#include "context.h"
#include "error_handler.h"


void UsageFault_Handler(void)
{
    printf("UsageFault_Handler\n");
}

void HardFault_Handler()
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
    SCB_Type *scb = SCB;

    uint32_t hfsr = scb->HFSR;
    uint32_t cfsr = scb->CFSR;

    printf("HardFault_Handler\n");
    printf("Stacked R0:\t\t%08x\n", (unsigned int) stack->R0);
    printf("Stacked R1:\t\t%08x\n", (unsigned int) stack->R1);
    printf("Stacked R2:\t\t%08x\n", (unsigned int) stack->R2);
    printf("Stacked R3:\t\t%08x\n", (unsigned int) stack->R3);
    printf("Stacked R12:\t\t%08x\n", (unsigned int) stack->R12);
    printf("Stacked LR:\t\t%08x\n", (unsigned int) stack->LR);
    printf("Stacked PC:\t\t%08x\n", (unsigned int) stack->PC);
    printf("Stacked xPSR:\t\t%08x\n", (unsigned int) stack->PSR);
    printf("SCB HFSR:\t\t%08x\n", (unsigned int) hfsr);
    printf("SCB CFSR:\t\t%08x\n", (unsigned int) cfsr);
    onboard_led_on(yellow_led);
    onboard_led_on(red_led);
    while (1);
}

void MemManager_Handler(void)
{
    printf("MemManager_Handler\n");
}

__attribute__((noreturn))
void Error_Handler(void)
{
    printf("Initialisation Error\n");
	onboard_led_on(yellow_led);
    onboard_led_on(red_led);
    while (1);
}
