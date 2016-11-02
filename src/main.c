#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "uart.h"
#include "mpu.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
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

	if (BSP_UART_init() != 0) {
		// Shit no working!
	}

	uint32_t counter = 0;

	init_mpu(0x20000000 + 0x2000, MPU_1KB);

	while (1) {
		printf("Hello world! : %"PRIu32"\n", counter++);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(1000);
	}
}
