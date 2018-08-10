/*
 * swtimer.h
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_SWTIMER_H_
#define SRC_HW_CORE_SWTIMER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_SWTIMER


#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL 0
#endif


#ifndef _HW_DEF_SW_TIMER_MAX
#define _HW_DEF_SW_TIMER_MAX        8
#endif


#define ONE_TIME            1
#define LOOP_TIME           2



typedef int16_t             swtimer_handle_t;



bool swtimerInit(void);
void swtimerSet  (uint8_t TmrNum, uint32_t TmrData, uint8_t TmrMode, void (*Fnct)(void *),void *arg);
void swtimerStart(uint8_t TmrNum);
void swtimerStop (uint8_t TmrNum);
void swtimerReset(uint8_t TmrNum);
void swtimerISR(void);


int16_t  swtimerGetHandle(void);
uint32_t swtimerGetCounter(void);


#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_SWTIMER_H_ */
