

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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//Enable the clock for TIM6 Peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();
	//Enable IRQ for TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	//Setup Priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
}
