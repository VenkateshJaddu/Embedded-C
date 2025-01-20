

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c;


void UART_init(void);
void SystemClockConfig(uint8_t);
void error_handler(void);
void send_msg(char[]);
void I2C_Init(void);

/* Callback Functions */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

/* LCD Function Prototypes */
void LCD_Init(void);
void LCD_Command(uint8_t);
void LCD_Data(uint8_t);
void LCD_Send(uint8_t , uint8_t);
void LCD_Print(char*);
void LCD_SetCursor(uint8_t , uint8_t );
void LCD_Clear(uint8_t);


#define SYS_CLK_FREQ_50_MHZ    50
#define SYS_CLK_FREQ_84_MHZ    84
#define SYS_CLK_FREQ_120_MHZ   120


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
