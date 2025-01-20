#include "main_1.h"

void SystemClockConfig(void);
void error_handler(void);
void Timer_init(void);
void GPIO_init(void);

TIM_HandleTypeDef timer6;

int main()
{
	HAL_Init();

	SystemClockConfig();
	Timer_init();
	GPIO_init();

	//Start the Timer
	HAL_TIM_Base_Start_IT(&timer6); //This function will automatically checks for status of TIM6->SR

	while(1);

	return 0;
}

void SystemClockConfig(void)
{

}

void Timer_init(void)
{
	timer6.Instance = TIM6;
	timer6.Init.Prescaler = PRESCALAR;
	timer6.Init.Period = PERIOD;

	if(HAL_TIM_Base_Init(&timer6) != HAL_OK)
	{
		error_handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void GPIO_init(void)
{
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Pin = GPIO_PIN_5;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);
}

void error_handler(void)
{
	printf("Failed\n");
	while(1);
}

