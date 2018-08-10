/*
 * button.c
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "button.h"


#ifdef _USE_HW_BUTTON


typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} button_tbl_t;



button_tbl_t button_tbl[BUTTON_MAX_CH] =
{
 {GPIOA, GPIO_PIN_0, GPIO_PIN_SET, GPIO_PIN_RESET},
};



void buttonInit(void)
{
  uint32_t i;
  GPIO_InitTypeDef  GPIO_InitStruct;


  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for (i=0; i<BUTTON_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = button_tbl[i].pin;
    HAL_GPIO_Init(button_tbl[i].port, &GPIO_InitStruct);
  }
}

bool buttonGetPressed(uint8_t ch)
{
  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  if (HAL_GPIO_ReadPin(button_tbl[ch].port, button_tbl[ch].pin) == button_tbl[ch].on_state)
  {
    return true;
  }
  else
  {
    return false;
  }
}


#endif
