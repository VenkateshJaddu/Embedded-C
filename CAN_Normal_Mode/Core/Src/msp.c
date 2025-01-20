#include "main_1.h"

void HAL_MspInit(void)
{
	/*Low level processor specific init configurations*/
	//1. Setup the priority grouping of ARM Cortex Mx Processor
			HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	//2. Enable the required system exceptions of ARM Cortex Mx Processor
			SCB->SHCSR |= 0x7 << 16;
	//3. Configuring the priority for the system exceptions
			HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0,0);
			HAL_NVIC_SetPriority(BusFault_IRQn, 0,0);
			HAL_NVIC_SetPriority(UsageFault_IRQn, 0,0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	/*Implementing Low level init's of USART2 peripheral*/
	//1. Enable the clock for the UART2 peripheral
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
	//2. Pin muxing configurations
		gpio_uart.Pin = GPIO_PIN_2;
		gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
		gpio_uart.Mode = GPIO_MODE_AF_PP;
		gpio_uart.Alternate = GPIO_AF7_USART2;
		gpio_uart.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &gpio_uart);

		gpio_uart.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3. Enable the IRQ and set-up the priority (NVIC Settings)
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef gpio_CAN;
	//Enable clock for CAN1
	__HAL_RCC_CAN1_CLK_ENABLE();

	//Pin Muxing Configuration
	gpio_CAN.Pin = GPIO_PIN_11;
	gpio_CAN.Mode = GPIO_MODE_AF_PP;
	gpio_CAN.Alternate = GPIO_AF9_CAN1;
	gpio_CAN.Pull = GPIO_NOPULL;
	gpio_CAN.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA,&gpio_CAN);

	gpio_CAN.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA,&gpio_CAN);

	//Enable the IRQ and set-up the priority (NVIC Settings)
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

	//HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);
}
