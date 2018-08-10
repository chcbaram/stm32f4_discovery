/*
 * button.c
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "button.h"
#include "swtimer.h"


#ifdef _USE_HW_BUTTON


typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} button_tbl_t;



button_tbl_t button_port_tbl[BUTTON_MAX_CH] =
{
 {GPIOA, GPIO_PIN_0, GPIO_PIN_SET, GPIO_PIN_RESET},
};


typedef struct
{
  bool        pressed;
  bool        pressed_event;
  uint16_t    pressed_cnt;
  uint32_t    pressed_start_time;
  uint32_t    pressed_end_time;

  bool        released;
  bool        released_event;
  uint32_t    released_start_time;
  uint32_t    released_end_time;

} button_t;


static button_t button_tbl[BUTTON_MAX_CH];

void button_isr(void *arg)
{
  uint8_t i;


  for (i=0; i<BUTTON_MAX_CH; i++)
  {

    if (buttonGetPressed(i))
    {
      if (button_tbl[i].pressed == false)
      {
        button_tbl[i].pressed_event = true;
        button_tbl[i].pressed_start_time = millis();
      }

      button_tbl[i].pressed = true;
      button_tbl[i].pressed_cnt++;

      button_tbl[i].pressed_end_time = millis();

      button_tbl[i].released = false;
    }
    else
    {
      if (button_tbl[i].pressed == true)
      {
        button_tbl[i].released_event = true;
        button_tbl[i].released_start_time = millis();
      }

      button_tbl[i].pressed  = false;
      button_tbl[i].released = true;

      button_tbl[i].released_end_time = millis();
    }
  }
}


void buttonInit(void)
{
  uint32_t i;
  GPIO_InitTypeDef  GPIO_InitStruct;
  swtimer_handle_t h_button_timer;


  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for (i=0; i<BUTTON_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = button_port_tbl[i].pin;
    HAL_GPIO_Init(button_port_tbl[i].port, &GPIO_InitStruct);
  }

  for (i=0; i<BUTTON_MAX_CH; i++)
  {
    button_tbl[i].pressed_cnt    = 0;
    button_tbl[i].pressed        = 0;
    button_tbl[i].released       = 0;
    button_tbl[i].released_event = 0;
  }

  h_button_timer = swtimerGetHandle();
  swtimerSet(h_button_timer, 1, LOOP_TIME, button_isr, NULL );
  swtimerStart(h_button_timer);
}

void buttonResetTime(uint8_t ch)
{
  button_tbl[ch].pressed_start_time    = 0;
  button_tbl[ch].pressed_end_time      = 0;
  button_tbl[ch].released_start_time   = 0;
  button_tbl[ch].released_end_time     = 0;
}

bool buttonGetPressed(uint8_t ch)
{
  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  if (HAL_GPIO_ReadPin(button_port_tbl[ch].port, button_port_tbl[ch].pin) == button_port_tbl[ch].on_state)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool buttonGetPressedEvent(uint8_t ch)
{
  bool ret;


  if (ch >= BUTTON_MAX_CH) return false;

  ret = button_tbl[ch].pressed_event;

  button_tbl[ch].pressed_event = 0;

  return ret;
}

uint32_t buttonGetPressedTime(uint8_t ch)
{
  volatile uint32_t ret;


  if (ch >= BUTTON_MAX_CH) return 0;


  ret = button_tbl[ch].pressed_end_time - button_tbl[ch].pressed_start_time;

  return ret;
}


bool buttonGetReleased(uint8_t ch)
{
  bool ret;


  if (ch >= BUTTON_MAX_CH) return false;

  ret = button_tbl[ch].released;

  return ret;
}

bool buttonGetReleasedEvent(uint8_t ch)
{
  bool ret;


  if (ch >= BUTTON_MAX_CH) return false;

  ret = button_tbl[ch].released_event;

  button_tbl[ch].released_event = 0;

  return ret;
}

uint32_t buttonGetReleasedTime(uint8_t ch)
{
  volatile uint32_t ret;


  if (ch >= BUTTON_MAX_CH) return 0;


  ret = button_tbl[ch].released_end_time - button_tbl[ch].released_start_time;

  return ret;
}
#endif
