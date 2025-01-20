
#include "main_1.h"
extern TIM_HandleTypeDef timer6;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_Callback(); //This may not be required
}

//IRQ names are Present in Startup file of Project
void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer6);
}
