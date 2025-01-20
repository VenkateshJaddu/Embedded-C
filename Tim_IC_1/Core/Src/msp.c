

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

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef timer2_gpio;
	//Enable the clock for Timer2 and GPIO which is used for Timer2_Channel1 Alternate function
	__HAL_RCC_TIM12_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//GPIO configuration
	timer2_gpio.Pin = GPIO_PIN_0;
	timer2_gpio.Mode = GPIO_MODE_AF_PP;
	timer2_gpio.Alternate = GPIO_AF1_TIM2;
	timer2_gpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &timer2_gpio);

	//NVIC settings and Priority
	HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
