#include "LCD_I2C.h"

void LCD_Init(LCD_I2C_HandleTypeDef *lcd, I2C_HandleTypeDef *hi2c, uint8_t cols, uint8_t rows, uint8_t SLAVE_ADDRESS){
	lcd->SLAVE_ADDRESS = SLAVE_ADDRESS;
	lcd->LCD_Backlight_Value = LCD_NOBACKLIGHT;
	lcd->LCD_Columns = cols;
	lcd->LCD_Rows = rows;
	lcd->hi2c = hi2c;
	lcd->LCD_Display_Option = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

	// 4 bit initialization
	HAL_Delay(50);  // wait for >40ms
	LCD_Send_Command(lcd, 0x30);
	HAL_Delay(5);  // wait for >4.1ms
	LCD_Send_Command(lcd, 0x30);
	HAL_Delay(1);  // wait for >100us
	LCD_Send_Command(lcd, 0x30);
	HAL_Delay(10);
	LCD_Send_Command(lcd, 0x20);  // 4bit mode
	HAL_Delay(10);

	// Display initialization
	LCD_Send_Command(lcd, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD_Send_Command(lcd, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	LCD_Send_Command(lcd, 0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_Send_Command(lcd, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_Send_Command(lcd, LCD_DISPLAYCONTROL | lcd->LCD_Display_Option); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void LCD_Write(LCD_I2C_HandleTypeDef *lcd, const char *str, ...){
	char stringArray[lcd->LCD_Columns];

	va_list args;
	va_start(args, str);
	vsprintf(stringArray, str, args);
	va_end(args);

	for(int i = 0; i < strlen(stringArray) && i < lcd->LCD_Columns; i++){
		LCD_Send_Data(lcd, stringArray[i]);
	}
}

void LCD_Write_Custom_Char(LCD_I2C_HandleTypeDef *lcd, char location){
	LCD_Send_Data(lcd, location);
}

void LCD_clear(LCD_I2C_HandleTypeDef *lcd){
	LCD_Send_Command(lcd, LCD_CLEARDISPLAY);
	HAL_Delay(2);
}

void LCD_Set_Cursor(LCD_I2C_HandleTypeDef *lcd, uint8_t col, uint8_t row){
	uint8_t row_Offets[] = {0x00, 0x40, 0x14, 0x54};
	if(row > lcd->LCD_Rows) row = lcd->LCD_Rows - 1;
	LCD_Send_Command(lcd, LCD_SETDDRAMADDR | (col + row_Offets[row]));
}

void LCD_Create_Char(LCD_I2C_HandleTypeDef *lcd, uint8_t location, uint8_t charMap[]){
	location &= 7; // only have 8 locations 0-7.
	location <<= 3;
	LCD_Send_Command(lcd, LCD_SETCGRAMADDR | location);
	for(int i = 0; i < 8; i++) LCD_Send_Data(lcd, charMap[i]);
}

void LCD_Backlight(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Backlight_Value = LCD_BACKLIGHT;
	LCD_Send_Command(lcd, 0);
}

void LCD_No_Backlight(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Backlight_Value = LCD_NOBACKLIGHT;
	LCD_Send_Command(lcd, 0);
}

void LCD_Cursor_Blink(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Display_Option |= LCD_BLINKON;
	LCD_Send_Command(lcd, LCD_DISPLAYCONTROL | lcd->LCD_Display_Option);
}

void LCD_Cursor_No_Blink(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Display_Option &= ~LCD_BLINKON;
	LCD_Send_Command(lcd, LCD_DISPLAYCONTROL | lcd->LCD_Display_Option);
}

void LCD_Display_On(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Display_Option |= LCD_DISPLAYON;
	LCD_Send_Command(lcd, LCD_DISPLAYCONTROL | lcd->LCD_Display_Option);
}

void LCD_Display_Off(LCD_I2C_HandleTypeDef *lcd){
	lcd->LCD_Display_Option &= ~LCD_DISPLAYON;
	LCD_Send_Command(lcd, LCD_DISPLAYCONTROL | lcd->LCD_Display_Option);
}

void LCD_Send_Command(LCD_I2C_HandleTypeDef *lcd, char cmd){
	char data_H, data_L;
	uint8_t I2C_Bufer[4];
	data_H = cmd & 0xF0;
	data_L = (cmd << 4) & 0xF0;

	I2C_Bufer[0] = data_H | lcd->LCD_Backlight_Value | En;
	I2C_Bufer[1] = data_H | lcd->LCD_Backlight_Value;
	I2C_Bufer[2] = data_L | lcd->LCD_Backlight_Value | En;
	I2C_Bufer[3] = data_L | lcd->LCD_Backlight_Value;

	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->SLAVE_ADDRESS, I2C_Bufer, 4, 100);
}

void LCD_Send_Data(LCD_I2C_HandleTypeDef *lcd, char data){
	char data_H, data_L;
	uint8_t I2C_Bufer[4];
	data_H = data & 0xF0;
	data_L = (data << 4) & 0xF0;

	I2C_Bufer[0] = data_H | lcd->LCD_Backlight_Value | En | Rs;
	I2C_Bufer[1] = data_H | lcd->LCD_Backlight_Value | Rs;
	I2C_Bufer[2] = data_L | lcd->LCD_Backlight_Value | En | Rs;
	I2C_Bufer[3] = data_L | lcd->LCD_Backlight_Value | Rs;

	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->SLAVE_ADDRESS, I2C_Bufer, 4, 100);
}

