#include "main_1.h"

UART_HandleTypeDef huart;
CAN_HandleTypeDef hcan;
I2C_HandleTypeDef hi2c;

/* Buffer for I2C Transmission */
uint8_t i2c_buffer[1];
volatile uint8_t i2c_busy = 0; // Flag to check if I2C is busy

int main()
{
	HAL_Init();

	SystemClockConfig(SYS_CLK_FREQ_50_MHZ);

	UART2_Init();

	I2C_Init();

    LCD_Init();
    send_msg("Initializations Done");

//    LCD_SetCursor(0,0);
    LCD_Print("Welcome......!");
    LCD_Clear(LCD_CLR_SCREEN);
//    LCD_Print("Test");

	CAN_Init(); // This will change the CAN controller from Sleep mode to Initialization mode

	CAN_Filter_Config(); // While Receiving CAN data we have to use filters HAL_CAN_ConfigFilter();

	//This will Enables bxCAN interrupts @arg CAN_Interrupts.
	if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF) != HAL_OK)
	{
		error_handler();
	}

	// This will change CAN from Initialization mode to Normal mode
	if(HAL_CAN_Start(&hcan) != HAL_OK)
	{
		error_handler();
	}
//	LCD_Print("CAN Tx");
	CAN_Tx();
//    Print_num(15);

	while(1);

	return 0;
}

void UART2_Init()
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

void I2C_Init(void)
{
	hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 100000;              // 100 kHz standard mode
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;     // Standard duty cycle
    hi2c.Init.OwnAddress1 = 0;                 // Not used in master mode
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c) != HAL_OK)
    {
        error_handler();
    }
}

/* I2C Transmission Complete Callback */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c1) {
    if (hi2c1->Instance == hi2c.Instance) {
        i2c_busy = 0; // Clear busy flag
//        send_msg("i2c Callback\r\n");
    }
}

/* I2C Error Callback */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c1) {
	send_msg("I2C Error Call\r\n");
    if (hi2c1->Instance == hi2c.Instance) {
        i2c_busy = 0; // Clear busy flag
        // Optional: Add error recovery logic here
    }
}



void SystemClockConfig(uint8_t clk_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint8_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clk_freq)
	{
	case SYS_CLK_FREQ_50_MHZ:
	{
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 100;
		osc_init.PLL.PLLP = 2;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
							RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_1WS;
		break;
	}

	case SYS_CLK_FREQ_84_MHZ:
	{
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 160;
		osc_init.PLL.PLLP = 2;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
							RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_2WS;
		break;
	}
	case SYS_CLK_FREQ_120_MHZ:
	{
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 240;
		osc_init.PLL.PLLP = 2;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
							RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_3WS;
		break;
	}
	default:
		return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
		{
			error_handler();
		}
	if(HAL_RCC_ClockConfig(&clk_init, FLatency) != HAL_OK)
		{
			error_handler();
		}


	/*
	 * Redo systick Configuration here as we configuring HCLK as 4MHZ here but before HCLK is 16MHZ.
	 * This will Wait for the crystal to stabilize.
	 * Also Ensure proper transitions between clock sources.
	 * */
	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}


void send_msg(char msg[])
{
    HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    char line[3];
    sprintf(line,"\r\n");
    HAL_UART_Transmit(&huart, (uint8_t *)line, strlen(line), HAL_MAX_DELAY);

}

void error_handler(void)
{
    send_msg("Entered into Error Handler");
	while(1);
}

