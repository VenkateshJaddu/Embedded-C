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
		gpio_uart.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &gpio_uart);

		gpio_uart.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3. Enable the IRQ and set-up the priority (NVIC Settings)
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef gpio_i2c;

    //if (hi2c->Instance == I2C1)
    //{
        // 1. Enable I2C clock
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        // 2. Configure I2C SCL and SDA pins (e.g., PB6 for SCL, PB7 for SDA)
        gpio_i2c.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        gpio_i2c.Mode = GPIO_MODE_AF_OD;       // Open-drain for I2C
        gpio_i2c.Pull = GPIO_PULLUP;
        gpio_i2c.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_i2c.Alternate = GPIO_AF4_I2C1;    // Alternate function for I2C1
        HAL_GPIO_Init(GPIOB, &gpio_i2c);

        // 3. Enable and configure NVIC for I2C interrupts (optional for interrupts)
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

        HAL_NVIC_SetPriority(I2C1_ER_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    //}
}

