#include "dht11.h"

/**
 * @brief Ця ф-ія дозволяє використати затримку у мілісекундах.
 */
void DHT11_DelayMicro(uint32_t microseconds)
{
    uint32_t counter = microseconds * (SystemCoreClock / 1000000) / 5;
    while (counter--)
        ;
}

void DHT11_SetMode(uint32_t mode, GPIO_TypeDef *Port, uint16_t Pin)
{
    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = Pin;
    GPIO_Init.Mode = mode;
    GPIO_Init.Pull = mode == GPIO_MODE_INPUT ? GPIO_PULLUP : GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Port, &GPIO_Init);
}

/**
 * @brief Зчитує та повертає значення температури та вологості
 *
 * @see
 */
void DHT11_Listen(float *temperature, uint8_t *humidity, GPIO_TypeDef *Port, uint16_t Pin)
{
    DHT11_SetMode(GPIO_MODE_OUTPUT_PP, Port, Pin);

    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_RESET);
    HAL_Delay(18);

    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_SET);
    DHT11_DelayMicro(20);

    DHT11_SetMode(GPIO_MODE_INPUT, Port, Pin);

    while (HAL_GPIO_ReadPin(Port, Pin))
        ;

    DHT11_DelayMicro(80);

    uint8_t buffer[5] = {0};

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 8; j++)
        {
            while (!HAL_GPIO_ReadPin(Port, Pin))
                ;
            DHT11_DelayMicro(30);

            if (HAL_GPIO_ReadPin(Port, Pin))
            {
                buffer[i] |= (1 << (7 - j));
            }

            while (HAL_GPIO_ReadPin(Port, Pin))
                ;
        }
    }

    if (buffer[4] != ((buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF))
    {
        *humidity = -128;
        *temperature = -128.0f;
        return;
    }

    *humidity = buffer[0];
    *temperature = buffer[2] + (float)buffer[3] / 10.0f;
}
