#include <stm32f4xx_hal.h>
#include <stdio.h>

#include "uart.h"


UART_HandleTypeDef UartHandle;


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	(void)huart;

	UARTx_TX_GPIO_CLK_ENABLE();
	UARTx_RX_GPIO_CLK_ENABLE();
	UARTx_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init = {
		.Pin       = UARTx_TX_PIN,
		.Mode      = GPIO_MODE_AF_PP,
		.Pull      = GPIO_PULLUP,
		.Speed     = GPIO_SPEED_FAST,
		.Alternate = UARTx_TX_AF,
	};
	HAL_GPIO_Init(UARTx_TX_GPIO_PORT, &gpio_init);
	gpio_init.Pin       = UARTx_RX_PIN;
	gpio_init.Alternate = UARTx_RX_AF;
	HAL_GPIO_Init(UARTx_RX_GPIO_PORT, &gpio_init);

	HAL_NVIC_SetPriority(UARTx_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UARTx_IRQn);
}


void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	(void)huart;

	UARTx_FORCE_RESET();
	UARTx_RELEASE_RESET();

	HAL_GPIO_DeInit(UARTx_TX_GPIO_PORT, UARTx_TX_PIN);
	HAL_GPIO_DeInit(UARTx_RX_GPIO_PORT, UARTx_RX_PIN);
}


int BSP_UART_init(void)
{
	HAL_UART_MspInit(&UartHandle);

	UartHandle.Instance = UARTx;

	UartHandle.Init.BaudRate     = 115200;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	return HAL_UART_Init(&UartHandle) == HAL_OK ? 0 : -1;
}


int BSP_UARTx_transmit(uint8_t* buf, size_t n)
{
	return HAL_UART_Transmit(&UartHandle, buf, n, 0xFFFF) == HAL_OK ? 0 : -1;
}


void BSP_UARTx_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}
