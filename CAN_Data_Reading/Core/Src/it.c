#include "main_1.h"
extern CAN_HandleTypeDef hcan;
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_Callback(); //This may not be required
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan);
}
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan);
}
void CAN1_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan);
}
void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan);
}
