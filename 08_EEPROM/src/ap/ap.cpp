/*
 * ap.cpp
 *
 *  Created on: 2018. 8. 9.
 *      Author: Baram
 */




#include "ap.h"



bool runWriteEeprom(void);
bool runReadEeprom(void);
bool runEraseEeprom(void);



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
        uartPrintf(_DEF_UART1, "w.. write eeprom\n");
        uartPrintf(_DEF_UART1, "r.. read  eeprom\n");
        uartPrintf(_DEF_UART1, "e.. erase eeprom\n");
        uartPrintf(_DEF_UART1, "\n");
      }

      if (ch == 'w')
      {
        runWriteEeprom();
      }
      if (ch == 'r')
      {
        runReadEeprom();
      }
      if (ch == 'e')
      {
        runEraseEeprom();
      }
    }
  }
}


bool runEraseEeprom(void)
{
  bool ret = true;
  uint8_t data[8] = {0, };


  uartPrintf(_DEF_UART1, "EraseEeprom..");
  if (eepromWrite(0, data, 8) == true)
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

bool runReadEeprom(void)
{
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "ReadEeprom..\n");

  for (int i=0; i<8; i++)
  {
    eepromRead(i, &data, 1);
    uartPrintf(_DEF_UART1, "0x%02X : 0x%02X \n", i, data);
  }

  return ret;
}

bool runWriteEeprom(void)
{
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "WriteEeprom..");

  for (uint32_t i=0; i<8; i++)
  {
    data = i;
    ret = eepromWrite(i, &data, 1);

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

