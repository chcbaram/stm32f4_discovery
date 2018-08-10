/*
 * ap.cpp
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "ap.h"


uint32_t flash_addr = 0x8020000;
uint32_t flash_size = 128 * 1024;



bool runEraseFlash(void);
bool runShowFlash(void);
bool runVerifyEmpty(void);
bool runTestWrite(void);



void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);
}

void apMain(void)
{
  while(1)
  {
    if (uartAvailable(_DEF_UART1) > 0)
    {
      uint8_t ch;

      ch = uartRead(_DEF_UART1);

      if (ch == 'h')
      {
        uartPrintf(_DEF_UART1, "h.. help\n");
        uartPrintf(_DEF_UART1, "e.. erase flash\n");
        uartPrintf(_DEF_UART1, "t.. test write\n");
        uartPrintf(_DEF_UART1, "s.. show flash\n");
        uartPrintf(_DEF_UART1, "v.. verify empty\n");
        uartPrintf(_DEF_UART1, "\n");
      }

      if (ch == 'e')
      {
        runEraseFlash();
      }
      if (ch == 's')
      {
        runShowFlash();
      }
      if (ch == 'v')
      {
        runVerifyEmpty();
      }
      if (ch == 't')
      {
        runTestWrite();
      }
    }
  }
}


bool runEraseFlash(void)
{
  bool ret = true;


  uartPrintf(_DEF_UART1, "EraseFlash..");
  if (flashErase(flash_addr, flash_size) == true)
  {
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else
  {
    uartPrintf(_DEF_UART1, "Fail\n");
    ret = false;
  }

  return ret;
}

bool runShowFlash(void)
{
  bool ret = true;
  uint32_t *p_flash = (uint32_t *)flash_addr;


  uartPrintf(_DEF_UART1, "ShowFlash..\n");

  for (int i=0; i<8; i++)
  {
    uartPrintf(_DEF_UART1, "0x%08X : 0x%08X \n", (int)&p_flash[i], p_flash[i]);
  }

  return ret;
}

bool runVerifyEmpty(void)
{
  bool ret = true;
  volatile uint32_t *p_flash = (uint32_t *)flash_addr;


  uartPrintf(_DEF_UART1, "VerifyEmpty..");

  for (uint32_t i=0; i<flash_size/4; i++)
  {
    if (p_flash[i] != 0xFFFFFFFF)
    {
      ret = false;
      break;
    }
  }

  if (ret == true)
  {
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else
  {
    uartPrintf(_DEF_UART1, "Fail\n");
  }

  return ret;
}

bool runTestWrite(void)
{
  bool ret = true;
  uint32_t *p_flash = (uint32_t *)flash_addr;


  uartPrintf(_DEF_UART1, "TestWrite..");

  for (uint32_t i=0; i<8; i++)
  {
    ret = flashWrite((uint32_t)&p_flash[i], (uint8_t *)&i, 4);

    if (ret == false)
    {
      break;
    }
  }

  if (ret == true)
  {
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else
  {
    uartPrintf(_DEF_UART1, "Fail\n");
  }

  return ret;
}


