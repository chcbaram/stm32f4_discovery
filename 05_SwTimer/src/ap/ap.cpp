/*
 * ap.cpp
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "ap.h"


void led_isr(void *arg)
{
  ledToggle(1);
}


void apInit(void)
{
  swtimer_handle_t h_led_timer;


  uartOpen(_DEF_UART1, 115200);


  h_led_timer = swtimerGetHandle();
  swtimerSet(h_led_timer, 500, LOOP_TIME, led_isr, NULL );
  swtimerStart(h_led_timer);
}

void apMain(void)
{
  while(1)
  {
    if (buttonGetPressedEvent(0) == true)
    {
      uartPrintf(_DEF_UART1, "PressedEvent\r\n");
    }
    if (buttonGetPressed(0) == true)
    {
      uartPrintf(_DEF_UART1, "PressedTime : %d\r\n", buttonGetPressedTime(0));
    }
    if (buttonGetReleasedEvent(0) == true)
    {
      uartPrintf(_DEF_UART1, "ReleasedEvent\r\n");
    }


    if (uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "Rxd : 0x%02X\r\n", uartRead(_DEF_UART1));
    }
  }
}

