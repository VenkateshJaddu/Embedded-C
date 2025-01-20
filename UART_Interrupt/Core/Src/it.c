

#include "main_1.h"

extern UART_HandleTypeDef huart;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_Callback(); //This may not be required
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart);
}
