
#include "main_1.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_Callback(); //This may not be required
}

void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c);
}

void I2C1_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hi2c);
}
