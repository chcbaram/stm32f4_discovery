/*
 * ap.cpp
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "ap.h"





void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);
}

void apMain(void)
{
  while(1)
  {
    if (buttonGetPressed(0) == true)
    {
      ledOn(0);
      uartPrintf(_DEF_UART1, "01234567890012345678900123456789001234567890012345678900123456789001234567890\r\n");
    }
    else
    {
      ledOff(0);
    }


    if (uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "Rxd : 0x%02X\r\n", uartRead(_DEF_UART1));
    }
  }
}

