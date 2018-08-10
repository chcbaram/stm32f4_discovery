/*
 * led.c
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "led.h"


#ifdef _USE_HW_LED


typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;



led_tbl_t led_tbl[LED_MAX_CH] =
{
 {GPIOD, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET},
 {GPIOD, GPIO_PIN_13, GPIO_PIN_SET, GPIO_PIN_RESET},
 {GPIOD, GPIO_PIN_14, GPIO_PIN_SET, GPIO_PIN_RESET},
 {GPIOD, GPIO_PIN_15, GPIO_PIN_SET, GPIO_PIN_RESET},
};



void ledInit(void)
{
  uint32_t i;
  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  for (i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = led_tbl[i].pin;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStruct);
  }
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH)
  {
    return;
  }

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH)
  {
    return;
  }

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}


#endif
