#include <inttypes.h>
#include "stdlib.h"
#include "lcd1602.h"

#define LCD_INIT 0b00110000
#define LCD_ADDR 0x4E

#define ROW_HEIGHT 0x40

#define MODE_ENABLE 0x04
#define MODE_DATA 0x01
#define MODE_COMMAND 0x00

#define CMD_BACKLIGHT 0x08
#define CMD_NO_BACKLIGHT 0x00

#define CMD_DISPLAY_ON 0x04
#define CMD_DISPLAY_OFF 0x00

#define CMD_CURSOR_ON 0x02
#define CMD_CURSOR_OFF 0x00

#define CMD_BLINK_ON 0x01
#define CMD_BLINK_OFF 0x00

#define CMD_CLEAR 0x01
#define CMD_HOME 0x02
#define CMD_SET_MODE 0x08

I2C_HandleTypeDef *lcd;

volatile uint32_t delay = 0;

volatile uint32_t lastPollingTime = 0;

/**
 * @brief This function transmits data to LCD.
 */
HAL_StatusTypeDef LCD_Transmit(uint8_t value, uint8_t mode)
{
	HAL_StatusTypeDef r;

	while (HAL_GetTick() - lastPollingTime < delay)
	{
		HAL_Delay(1);
	}

	for (;;)
	{
		r = HAL_I2C_IsDeviceReady(lcd, LCD_ADDR, 1, HAL_MAX_DELAY);

		if (r == HAL_OK)
		{
			break;
		}
	}

	uint8_t upperNibble = value & 0xF0;
	uint8_t lowerNibble = (value << 4) & 0xF0;

	uint8_t txBuffer[4];

	txBuffer[0] = upperNibble | mode | CMD_BACKLIGHT | MODE_ENABLE;
	txBuffer[1] = upperNibble | mode | CMD_BACKLIGHT;
	txBuffer[2] = lowerNibble | mode | CMD_BACKLIGHT | MODE_ENABLE;
	txBuffer[3] = lowerNibble | mode | CMD_BACKLIGHT;

	r = HAL_I2C_Master_Transmit(lcd, LCD_ADDR, txBuffer, sizeof(txBuffer), HAL_MAX_DELAY);

	lastPollingTime = HAL_GetTick();

	return r;
}

/**
 * @brief This function transmits command to LCD.
 */
HAL_StatusTypeDef LCD_TransmitCommand(uint8_t cmd)
{
	HAL_StatusTypeDef r = LCD_Transmit(cmd, MODE_COMMAND);

	if (r == HAL_OK)
	{
		delay = 5;
	}

	return r;
}

/**
 * @brief This function initializes LCD.
 */
HAL_StatusTypeDef LCD_Init(I2C_HandleTypeDef *i2c)
{
	lcd = i2c;

	while (HAL_GetTick() < 50)
	{
		HAL_Delay(1);
	}

	HAL_StatusTypeDef r = HAL_OK;

	r += LCD_TransmitCommand(LCD_INIT);
	r += LCD_TransmitCommand(LCD_INIT);
	r += LCD_TransmitCommand(LCD_INIT);

	r += LCD_TransmitCommand(CMD_HOME);
	r += LCD_TransmitCommand(CMD_SET_MODE | CMD_DISPLAY_ON | CMD_CURSOR_OFF | CMD_BLINK_OFF);
	r += LCD_TransmitCommand(CMD_CLEAR);

	return r;
}

/**
 * @brief This function clears LCD display.
 */
HAL_StatusTypeDef LCD_Clear()
{
	return LCD_TransmitCommand(CMD_CLEAR);
}

/**
 * @brief This functions changes cursor position.
 */
HAL_StatusTypeDef LCD_SetCursor(uint8_t x, uint8_t y)
{
	return LCD_TransmitCommand(0x80 | (x + y * ROW_HEIGHT));
}

/**
 * @brief This function writes a single char to LCD.
 */
HAL_StatusTypeDef LCD_WriteChar(char *c)
{
	return LCD_Transmit((uint8_t)(*c), MODE_DATA);
}

/**
 * @brief This function writes a string to LCD.
 */
HAL_StatusTypeDef LCD_Write(char *c)
{
	HAL_StatusTypeDef r = HAL_OK;

	while (*c != '\0')
	{
		r += LCD_WriteChar(c);
		c++;
	}

	return r;
}
