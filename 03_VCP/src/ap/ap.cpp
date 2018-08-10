/*
 * ap.cpp
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "ap.h"





void apInit(void)
{

}

void apMain(void)
{
  while(1)
  {
    if (buttonGetPressed(0) == true)
    {
      ledOn(0);
      vcpPrintf("01234567890012345678900123456789001234567890012345678900123456789001234567890\r\n");
    }
    else
    {
      ledOff(0);
    }
  }
}
