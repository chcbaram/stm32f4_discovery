/*
 * flash.c
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */




#include "flash.h"


#ifdef _USE_HW_FLASH

#define FLASH_MAX_PAGE_COUNT    12





typedef struct
{
  uint32_t address;
  uint32_t length;
} flash_tbl_t;



flash_tbl_t flash_tbl[FLASH_MAX_PAGE_COUNT];



static int32_t getPage(uint32_t Address);



void flashInit(void)
{

  flash_tbl[0].address = 0x08000000;
  flash_tbl[0].length  = 16*1024;

  flash_tbl[1].address = 0x08004000;
  flash_tbl[1].length  = 16*1024;

  flash_tbl[2].address = 0x08008000;
  flash_tbl[2].length  = 16*1024;

  flash_tbl[3].address = 0x0800C000;
  flash_tbl[3].length  = 16*1024;

  flash_tbl[4].address = 0x08010000;
  flash_tbl[4].length  = 64*1024;

  flash_tbl[5].address = 0x08020000;
  flash_tbl[5].length  = 128*1024;

  flash_tbl[6].address = 0x08040000;
  flash_tbl[6].length  = 128*1024;

  flash_tbl[7].address = 0x08060000;
  flash_tbl[7].length  = 128*1024;

  flash_tbl[8].address = 0x08080000;
  flash_tbl[8].length  = 128*1024;

  flash_tbl[9].address = 0x080A0000;
  flash_tbl[9].length  = 128*1024;

  flash_tbl[10].address = 0x080C0000;
  flash_tbl[10].length  = 128*1024;

  flash_tbl[11].address = 0x080E0000;
  flash_tbl[11].length  = 128*1024;

}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = true;
  int32_t first_page = 0;
  int32_t num_page = 0;
  uint32_t SECTORError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;



  HAL_FLASH_Unlock();



  first_page = getPage(addr);
  num_page   = getPage(addr + length - 1) - first_page + 1;


  if (first_page < 0)
  {
    return false;
  }

  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = first_page;
  EraseInitStruct.NbSectors = num_page;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
  {
    ret = false;
  }

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  __HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();

  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();

  HAL_FLASH_Lock();


  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint64_t data;


  // 2byte align
  if (addr%2 != 0)
  {
    return false;
  }

  HAL_FLASH_Unlock();

  for (uint32_t i=0; i<length; i += 2)
  {
    data = (p_data[i+1] << 8) | (p_data[i+0] << 0);

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, data) != HAL_OK)
    {
      ret = false;
      break;
    }
  }


  HAL_FLASH_Lock();

  return ret;
}

static int32_t getPage(uint32_t address)
{
  uint32_t i;
  int32_t page = -1;


  for (i=0; i<FLASH_MAX_PAGE_COUNT; i++)
  {
    if (address >= flash_tbl[i].address && address < (flash_tbl[i].address + flash_tbl[i].length))
    {
      page = i;
      break;
    }
  }

  return page;
}


#endif
