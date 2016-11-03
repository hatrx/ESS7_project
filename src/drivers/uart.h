#ifndef UART_H
#define UART_H


#include <stm32f4xx_hal.h>


#define UARTx                           UART4
#define UARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define UARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define UARTx_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define UARTx_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

/* Definition for UARTx Pins */
#define UARTx_TX_PIN                    GPIO_PIN_10
#define UARTx_TX_GPIO_PORT              GPIOC
#define UARTx_TX_AF                     GPIO_AF8_UART4
#define UARTx_RX_PIN                    GPIO_PIN_11
#define UARTx_RX_GPIO_PORT              GPIOC
#define UARTx_RX_AF                     GPIO_AF8_UART4

/* Definition for UARTx's NVIC */
#define UARTx_IRQn                      UART4_IRQn
#define UARTx_IRQHandler                UART4_IRQHandler


void HAL_UART_MspInit     (UART_HandleTypeDef *huart);
int  BSP_UART_init        (void);
void BSP_UARTx_IRQHandler (void);
int  BSP_UARTx_transmit   (uint8_t* buf, size_t n);


#endif /* UART_H */
