#ifndef __HAL_STM32_DHT_H__
#define __HAL_STM32_DHT_H__

#include "stm32f4xx_hal.h"

void DHT11_DelayMicro(uint32_t microseconds);
void DHT11_Listen(float *temperature, uint8_t *humidity, GPIO_TypeDef *Port, uint16_t Pin);

#endif
