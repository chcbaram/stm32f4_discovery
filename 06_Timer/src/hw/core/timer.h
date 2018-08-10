/*
 * timer.h
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_TIMER_H_
#define SRC_HW_CORE_TIMER_H_


#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_TIMER

#define TIMER_MAX_CH    4



void timerInit(void);
void timerStop(uint8_t channel);
void timerSetPeriod(uint8_t channel, uint32_t period_us);
void timerAttachInterrupt(uint8_t channel, void (*func)(void));
void timerDetachInterrupt(uint8_t channel);
void timerStart(uint8_t channel);


#endif

#ifdef __cplusplus
}
#endif


#endif /* SRC_HW_CORE_TIMER_H_ */
