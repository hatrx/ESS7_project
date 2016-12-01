#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <apex_process.h>
#include <string.h>

#include "drivers/uart.h"
#include "drivers/mpu.h"
#include "drivers/onboard_leds.h"
#include "drivers/system_clock.h"
#include "drivers/watchdog.h"
#include "drivers/time_get.h"
#include "drivers/rtc.h"
#include "drivers/utils.h"

#include "kernel/context.h"

#include "kernel/arinc/partition.h"
#include "kernel/arinc/statics.h"
#include "kernel/arinc/queuing_port.h"
#include "kernel/arinc/process.h"

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
    while (1)
	;
}

void MemManager_Handler(void)
{
    printf("MemManager_Handler\n");
}

void Error_Handler(void)
{
    printf("Initialisation Error\n");
    while (1)
	;
}

int main(void)
{
    /* Disable sysTick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    PROCESS_ATTRIBUTE_TYPE dummy1_mainProcess_attributes, dummy2_mainProcess_attributes, stdio_sys_mainProcess_attributes;
    PROCESS_ID_TYPE dummy1_pid, dummy2_pid, stdio_sys_pid;
	RETURN_CODE_TYPE RETURN_CODE;

    set_system_clock_168mhz();

    if (BSP_UART_Init() != 0)
    {
	Error_Handler(); // Shit not working!
    }

    init_onboard_LED(red_led);
    init_onboard_LED(yellow_led);

    // Clear the UART terminal windows (if it supports ANSI Escape codes)
    printf("%c[%dJ", 0x1B, 2);
    printf("%c[0;0H", 0x1B);
    /*
	if (BSP_IWDG_Init() != 0)		//Independent watchdog time initilisation
	{
		Error_Handler();		// Shit not working!
	}

	if (BSP_RTC_Init() != 0)		//Real time clock initialisation. Define the time interval in the header file
	{
		Error_Handler();		// Shit not working!
	}
	*/
    //init_mpu(0x20000000 + 0x2000, MPU_1KB);
    /* Initialize all ports */
    init_queuing_ports();

    dummy1_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE){
	.PERIOD = 0,
	.TIME_CAPACITY = 0,
	.ENTRY_POINT = &dummy1_main,
	.BASE_PRIORITY = 1,
	.DEADLINE = SOFT,
	.NAME = "dummy1",
    };

    dummy2_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE){
	.PERIOD = 0,
	.TIME_CAPACITY = 0,
	.ENTRY_POINT = &dummy2_main,
	.BASE_PRIORITY = 1,
	.DEADLINE = SOFT,
	.NAME = "dummy2",
    };

    stdio_sys_mainProcess_attributes = (PROCESS_ATTRIBUTE_TYPE){
	.PERIOD = 0,
	.TIME_CAPACITY = 0,
	.ENTRY_POINT = &stdio_sys_main,
	.BASE_PRIORITY = 1,
	.DEADLINE = SOFT,
	.NAME = "stdio_sys",
    };
    init_partitions();
    create_process(&partitions[0], 0x20001000, &dummy1_mainProcess_attributes, &dummy1_pid, &RETURN_CODE);
    create_process(&partitions[1], 0x20003000, &dummy2_mainProcess_attributes, &dummy2_pid, &RETURN_CODE);
    create_process(&partitions[2], 0x20005000, &stdio_sys_mainProcess_attributes, &stdio_sys_pid, &RETURN_CODE);
    init_partition(&partitions[0]);
    init_partition(&partitions[1]);
    init_partition(&partitions[2]);
    TIME_Start_ns();

    /* Enable sysTick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    __asm volatile (
		"MRS R4, CONTROL			\n\t"
		"ORR R4, R4, #0x1			\n\t"
		"MSR CONTROL, R4			\n\t"
		: : :
	);

    while (1)
    {
    }
}
