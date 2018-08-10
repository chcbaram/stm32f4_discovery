/*
 * flash.h
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_FLASH_H_
#define SRC_HW_CORE_FLASH_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


#ifdef _USE_HW_FLASH

void flashInit(void);

bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);

#endif


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_FLASH_H_ */
