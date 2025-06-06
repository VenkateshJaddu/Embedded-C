#include "main_1.h"
#include <string.h>

void UART2_Init(void);
void SystemClockConfig(uint8_t);
void error_handler(void);
void CAN_Init(void);
void CAN_Tx(void);
void CAN_Filter_Config(void);
void CAN_Rx(void);

UART_HandleTypeDef huart;
CAN_HandleTypeDef hcan;

int main()
{
	char msg[30];
	HAL_Init();

	SystemClockConfig(SYS_CLK_FREQ_50_MHZ);

	UART2_Init();

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
	CAN_Tx();

	sprintf(msg,"\r\nReady to Receive ID's\r\n");
	HAL_UART_Transmit(&huart,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);


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

void CAN_Init(void)
{
	hcan.Instance = CAN1;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;

	//Time Quanta Parameter settings
	hcan.Init.Prescaler = 5;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_1TQ;

	if(HAL_CAN_Init(&hcan) != HAL_OK)
		{
			error_handler();
		}

}

void CAN_Tx(void)
{
	CAN_TxHeaderTypeDef CAN_Tx;
	uint32_t TxMailbox;
	//uint8_t data_msg[5] = {'H','E','L','L','0'};
	uint8_t data_msg[8] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

	CAN_Tx.StdId = 0x620;
	CAN_Tx.IDE = CAN_ID_STD;
	CAN_Tx.RTR = CAN_RTR_DATA;
	CAN_Tx.DLC = 8;

	if(HAL_CAN_AddTxMessage(&hcan, &CAN_Tx, data_msg, &TxMailbox) != HAL_OK)
		{
			error_handler();
		}
}

void CAN_Rx(void)
{
	CAN_RxHeaderTypeDef CAN_Rx;
	uint8_t Rec_data[8];
	char msg[50];

	//Wait until atleast one message filled in CAN_RX_FIFO0
	while(!(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)));

	if(HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0, &CAN_Rx, Rec_data) != HAL_OK)
	{
		error_handler();
	}

	// Print each byte of received data
	    for (int i = 0; i < 8; i++)
	    {
	        sprintf(msg, "0x%02X ", Rec_data[i]);
	        HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	    }

	    sprintf(msg, "\r\n");
	    HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);


	//Adding Null Terminator
	 //Rec_data[5] = '\0';

	/*
	 HAL_UART_Transmit(&huart,(uint8_t *)Rec_data,sizeof(Rec_data),HAL_MAX_DELAY);

	const char *newline = "\r\n";
	HAL_UART_Transmit(&huart, (uint8_t *)newline, strlen(newline), HAL_MAX_DELAY);
	*/

}

void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef CAN_Filter;

	CAN_Filter.FilterActivation = CAN_FILTER_ENABLE;
	CAN_Filter.FilterIdHigh = 0x620 << 5;
	CAN_Filter.FilterIdLow = 0x0000; //For Standard ID's(11bit) Low filter ID is not used
	CAN_Filter.FilterMaskIdHigh = 0x7F0 << 5;
	CAN_Filter.FilterMaskIdLow = 0x0000; //For Standard ID's(11bit) Low filter MaskID is not used
	CAN_Filter.FilterFIFOAssignment = CAN_RX_FIFO0; //Can use CAN_FILTER_FIFO0 as well
	CAN_Filter.FilterBank = 0;
	CAN_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_Filter.FilterScale = CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&hcan, &CAN_Filter) != HAL_OK)
		{
			error_handler();
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

/*CAN Interrupt Callbacks for TX & RX*/

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[35];
	sprintf(msg,"\r\nMessage Transmitted:Mailbox0\r\n");
	HAL_UART_Transmit(&huart,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[35];
	sprintf(msg,"\r\nMessage Transmitted:Mailbox1\r\n");
	HAL_UART_Transmit(&huart,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[35];
	sprintf(msg,"\r\nMessage Transmitted:Mailbox2\r\n");
	HAL_UART_Transmit(&huart,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef CAN_RxHeader;
	uint8_t Rec_data[8];
	char msg[100];

	CAN_RxHeader.IDE = CAN_ID_STD;
	CAN_RxHeader.RTR = CAN_RTR_DATA;
	CAN_RxHeader.DLC = 8;

	if(HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0, &CAN_RxHeader, Rec_data) != HAL_OK)
	{
		error_handler();
	}

	if(CAN_RxHeader.StdId == 0x621)
	{
		CAN_Tx();

		// Print each byte of received data
	    for (int i = 0; i < 8; i++)
	    {
	        sprintf(msg, "0x%02X ", Rec_data[i]);
	        HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	    }

	    sprintf(msg, "\r\n");
	    HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	}
	else
	{
	    sprintf(msg, "0x621 Not received");
	    HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char msg[20];
	sprintf(msg,"\r\nError Occurred\r\n");
	HAL_UART_Transmit(&huart,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}


void error_handler(void)
{
	char msg[35];
    sprintf(msg, "\r\nEntered into Error Handler\r\n");
    HAL_UART_Transmit(&huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	while(1);
}

