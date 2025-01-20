#include "main_1.h"

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
//	hcan.Init.Prescaler = 5; //To get 250Kbps Pre-scalar is 10
	hcan.Init.Prescaler = 10;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_1TQ;

/*	hcan.Init.Prescaler = 6; //To get 500Kbps Pre-scalar is 5
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_11TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
*/

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

	CAN_Tx.StdId = 0x330;
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

/*CAN Interrupt Callbacks for TX & RX*/

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	send_msg("\r\nMessage Transmitted:Mailbox0\r\n");
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	send_msg("Message Transmitted:Mailbox1");
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	send_msg("Message Transmitted:Mailbox2");
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef CAN_RxHeader;
	uint8_t Rec_data[8];

	CAN_RxHeader.IDE = CAN_ID_STD;
	CAN_RxHeader.RTR = CAN_RTR_DATA;
	CAN_RxHeader.DLC = 8;

	send_msg("CAN Rx");

	if(HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0, &CAN_RxHeader, Rec_data) != HAL_OK)
	{
		error_handler();
	}
	//CAN_Tx();
	//uint32_t ID = (uint32_t)CAN_RxHeader.StdId;
	LCD_Print("CAN Rx");

	switch(CAN_RxHeader.StdId)
	{
		case 0x621:
			send_msg("621 ID");
			speed(Rec_data);
			break;
		case 0x620:
			send_msg("620 ID");
			Mode(Rec_data);
			break;
		default:
			send_msg("Switch entered default state");
			break;
	}
}

uint32_t speed_val,speed_kmph;
void speed(uint8_t *arr)
{
	LCD_Clear(LCD_CLR_SCREEN);
	char speed[5];
	speed_val = arr[2] << 8 | arr[3];
	speed_kmph = (uint32_t)(0.1183f * (float)speed_val);
	send_msg("\r\nSpeed in KMPH : ");
    sprintf(speed,"%lu",speed_kmph);
    LCD_Print(speed);
    HAL_UART_Transmit(&huart,(uint8_t *)speed, sizeof(speed), HAL_MAX_DELAY);
    LCD_Clear(LCD_CLR_SCREEN);
}

void Mode(uint8_t *arr)
{
	LCD_Clear(LCD_CLR_SCREEN);
		if(arr[2] == 38)
		{
			send_msg("\r\nMode : ECO \r\n");
			LCD_Print("MODE : ECO");
		}
		else if(arr[2] == 70)
		{
			send_msg("\r\nMode : Normal \r\n");
			LCD_Print("MODE : Normal");
		}
		else if(arr[2] == 134)
		{
			send_msg("\r\nMode : Sports \r\n");
			LCD_Print("MODE : Sports");
		}
		else if(arr[2] == 96 && speed_kmph == 0)
		{
			send_msg("\r\n ...Parking... \r\n");
			LCD_Print("...Parking...");
		}
		else if(arr[3] == 128)
		{
			send_msg("\r\n ...Reverse... \r\n");
			LCD_Print("...Reverse...");
		}
		else
		{
			error_handler();
		}

}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	send_msg("Error Occurred");
}

