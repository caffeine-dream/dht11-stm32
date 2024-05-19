#ifndef __HAL_STM32_LCD1602_H__
#define __HAL_STM32_LCD1602_H__

#include <inttypes.h>
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef LCD_Init(I2C_HandleTypeDef *i2c);
HAL_StatusTypeDef LCD_Clear();
HAL_StatusTypeDef LCD_SetCursor(uint8_t x, uint8_t y);
HAL_StatusTypeDef LCD_WriteChar(char *c);
HAL_StatusTypeDef LCD_Write(char *t);

#endif
