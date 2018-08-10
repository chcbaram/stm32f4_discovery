/*
 * uart.h
 *
 *  Created on: 2018. 8. 10.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_UART_H_
#define SRC_HW_CORE_UART_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_UART


#define UART_MAX_CH     1


void     uartInit(void);
bool     uartOpen(uint8_t channel, uint32_t baud);
bool     uartClose(uint8_t channel);
uint32_t uartAvailable(uint8_t channel);
void     uartFlush(uint8_t channel);
void     uartPutch(uint8_t channel, uint8_t ch);
uint8_t  uartGetch(uint8_t channel);
int32_t  uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length);
uint8_t  uartRead(uint8_t channel);
int32_t  uartPrintf(uint8_t channel, const char *fmt, ...);



#endif

#ifdef __cplusplus
}
#endif


#endif /* SRC_HW_CORE_UART_H_ */
