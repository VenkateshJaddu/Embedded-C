#include "main_1.h"

void LCD_Init(void) {
    HAL_Delay(50); // Wait for LCD to power up

    // Initialization sequence
    LCD_Send(0x03, 0); // Function set (8-bit mode)
    HAL_Delay(5);      // Wait >4.1ms
    LCD_Send(0x03, 0); // Repeat
    HAL_Delay(1);      // Wait >100us
    LCD_Send(0x03, 0); // Repeat
    LCD_Send(0x02, 0); // Set 4-bit mode

    // Configure LCD
    LCD_Command(LCD_FUNCTION_SET);  // 4-bit mode, 2-line display
    LCD_Command(LCD_DISPLAY_ON);    // Display ON, Cursor OFF
    LCD_Command(LCD_CLEAR);         // Clear display
    LCD_Command(LCD_ENTRY_MODE);    // Increment cursor, no shift
}

void LCD_Command(uint8_t cmd) {
    LCD_Send(cmd, 0); // Send as command (RS = 0)
}

void LCD_Data(uint8_t data) {
    LCD_Send(data, LCD_RS); // Send as data (RS = 1)
}


/* Send Data/Command with Mode (RS) */
void LCD_Send(uint8_t value, uint8_t mode) {
    uint8_t upper_nibble = (value & 0xF0) | mode | LCD_EN | LCD_BACKLIGHT;
    uint8_t lower_nibble = ((value << 4) & 0xF0) | mode | LCD_EN | LCD_BACKLIGHT;

//    if (HAL_I2C_GetState(&hi2c) != HAL_I2C_STATE_READY) {
//        Handle_I2C_Busy();
//    }
    // Send upper nibble
    i2c_busy = 1;
    i2c_buffer[0] = upper_nibble;
//    HAL_I2C_Master_Transmit_IT(&hi2c, LCD_ADDR, i2c_buffer, 1);
//    while (i2c_busy);
    if (HAL_I2C_Master_Transmit_IT(&hi2c, LCD_ADDR, i2c_buffer, 1) == HAL_OK) {
        uint32_t timeout = HAL_GetTick() + 10;  // 10ms timeout
        while (i2c_busy) {
            if (HAL_GetTick() > timeout) {
                send_msg("I2C timeout on upper nibble\r\n");
                return;
            }
        }
    } else {
        send_msg("I2C transmission error on upper nibble\r\n");
        return;
    }

//    send_msg("Upper nibble 1\r\n");
//    if (HAL_I2C_GetState(&hi2c) != HAL_I2C_STATE_READY) {
//        Handle_I2C_Busy();
//    }

    i2c_busy = 1;
    i2c_buffer[0] = upper_nibble & ~LCD_EN;
    HAL_I2C_Master_Transmit_IT(&hi2c, LCD_ADDR, i2c_buffer, 1);
    while (i2c_busy);
//    send_msg("Upper nibble 2\r\n");

    // Send lower nibble
    i2c_busy = 1;
    i2c_buffer[0] = lower_nibble;
    HAL_I2C_Master_Transmit_IT(&hi2c, LCD_ADDR, i2c_buffer, 1);
    while (i2c_busy);

    i2c_busy = 1;
    i2c_buffer[0] = lower_nibble & ~LCD_EN;
    HAL_I2C_Master_Transmit_IT(&hi2c, LCD_ADDR, i2c_buffer, 1);
    while (i2c_busy);
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_Data((uint8_t)*str++);
    }
}

/* Set Cursor Position */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? col : (0x40 + col);
    LCD_Command(LCD_SET_DDRAM_ADDR | addr);
}

void LCD_Clear(uint8_t cmd) {
    //LCD_Send(cmd, 0);  // 0x01 is the clear screen command
    LCD_Command(cmd);
    HAL_Delay(2);       // Wait for 2ms to ensure the command is processed
}

void Print_num(uint8_t num) {
    char buffer[15];  // Buffer to hold the string representation of the number (enough for "255" + null terminator)
    uint8_t i;

    for (i = 0; i < num; i++) {
        // Convert the number to a string
        snprintf(buffer, sizeof(buffer), "%u", i);

        // Print the string to the LCD
        LCD_Print(buffer);

        // Optional: Add a delay to observe the number before clearing
        HAL_Delay(200);

        // Clear the LCD screen
        LCD_Clear(LCD_CLR_SCREEN);
    }
}

/*void Print_speed(uint8_t speed)
{
	char speed_1[1];
	snprintf(speed_1, sizeof(speed_1), "%u", speed);
	LCD_Print(speed_1);
	LCD_Clear(LCD_CLR_SCREEN);
}
*/
