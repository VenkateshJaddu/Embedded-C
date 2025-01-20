

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"

/*Handlers*/
extern UART_HandleTypeDef huart;
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c;

extern uint8_t i2c_buffer[1];
extern volatile uint8_t i2c_busy;

#define SYS_CLK_FREQ_50_MHZ    50
#define SYS_CLK_FREQ_84_MHZ    84
#define SYS_CLK_FREQ_120_MHZ   120


void UART2_Init(void);
void SystemClockConfig(uint8_t);
void error_handler(void);
void I2C_Init(void);

/*CAN Functions*/
void CAN_Init(void);
void CAN_Tx(void);
void CAN_Filter_Config(void);
void CAN_Rx(void);
void send_msg(char[]);
void speed(uint8_t*);
void Mode(uint8_t*);


/* Callback Functions */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void Print_num(uint8_t);

/* LCD Function Prototypes */
void LCD_Init(void);
void LCD_Command(uint8_t);
void LCD_Data(uint8_t);
void LCD_Send(uint8_t , uint8_t);
void LCD_Print(char*);
void LCD_SetCursor(uint8_t , uint8_t );
void LCD_Clear(uint8_t);

/* LCD Commands */
#define LCD_ADDR			 (0x27 << 1)

#define LCD_CLEAR            0x01
#define LCD_HOME             0x02
#define LCD_ENTRY_MODE       0x06
#define LCD_DISPLAY_ON       0x0C
#define LCD_DISPLAY_OFF      0x08
#define LCD_FUNCTION_SET     0x28
#define LCD_SET_DDRAM_ADDR   0x80
#define LCD_CLR_SCREEN		 0x01
#define LCD_BACKLIGHT 		 0x08  // Backlight ON
#define LCD_NOBACKLIGHT 	 0x00 // Backlight OFF

/* Control Bits */
#define LCD_RS  (1 << 0) // Register Select: 0 = Command, 1 = Data
#define LCD_RW  (1 << 1) // Read/Write: 0 = Write, 1 = Read
#define LCD_EN  (1 << 2) // Enable

#endif
