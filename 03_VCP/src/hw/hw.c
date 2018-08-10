/*
 * hw.c
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "hw.h"





void hwInit(void)
{
  bspInit();

  usbInit();
  vcpInit();
  ledInit();
  buttonInit();
}
