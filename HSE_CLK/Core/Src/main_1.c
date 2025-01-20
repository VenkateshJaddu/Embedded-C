#include <stdio.h>
#include <string.h>
#include "main_1.h"
#include "stm32f4xx_hal.h"

void UART_init(void);
void SystemClockConfig(void);
void error_handler(void);

UART_HandleTypeDef huart;


int main()
{
	char msg[100];

	RCC_ClkInitTypeDef clk_init;
	RCC_OscInitTypeDef osc_init;

	HAL_Init();
	UART_init();

	memset(&osc_init, 0, sizeof(osc_init)); //This will initialize RCC_OscInitTypeDef structure to zero
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init,FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		error_handler();
	}
	__HAL_RCC_HSI_DISABLE();

	/*
	 * Redo systick Configuration here as we configuring HCLK as 4MHZ here but before HCLK is 16MHZ.
	 * This will Wait for the crystal to stabilize.
	 * Also Ensure proper transitions between clock sources.
	 * */
	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/*As PCLK1 & PCLK2 freq is changed to 2MHZ we have to re-initialize UART*/
	UART_init();

	/*Lets Print all CLK frequencies*/
	memset(&msg,0,sizeof(msg));
	sprintf(msg, "SYSCLK : %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart, (uint8_t *)msg,sizeof(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg, "HCLK : %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart, (uint8_t *)msg,sizeof(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg, "PCLK1 : %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart, (uint8_t *)msg,sizeof(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg, "PCLK2 : %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart, (uint8_t *)msg,sizeof(msg), HAL_MAX_DELAY);

	while(1);

	return 0;
}

void UART_init()
{
	huart.Instance = USART2;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	if((HAL_UART_Init(&huart)) != HAL_OK)
	{
		error_handler();
	}
}

void error_handler(void)
{
	printf("Failed\n");
	while(1);
}

