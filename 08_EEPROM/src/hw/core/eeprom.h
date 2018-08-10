/*
 * eeprom.h
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_EEPROM_H_
#define SRC_HW_CORE_EEPROM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_EEPROM



bool eepromInit();


bool  eepromRead(uint32_t addr, uint8_t *p_data, uint32_t length);
bool  eepromWrite(uint32_t addr, uint8_t *p_data, uint32_t length);

uint8_t  eepromReadByte(uint32_t addr);
bool     eepromWriteByte(uint32_t addr, uint8_t data_in);
uint32_t eepromGetLength(void);
bool     eepromFormat(void);



#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_EEPROM_H_ */
