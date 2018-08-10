/*
 * timer.c
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */




#include "timer.h"


#ifdef _USE_HW_TIMER

#define HWTIMER_MAX_CH    1

#define HWTIMER_TIMER1    0
#define HWTIMER_TIMER2    1
#define HWTIMER_TIMER3    2
#define HWTIMER_TIMER4    3

#define HWTIMER_CH1       0
#define HWTIMER_CH2       1
#define HWTIMER_CH3       2
#define HWTIMER_CH4       3



typedef struct
{
  TIM_HandleTypeDef    hTIM;
  TIM_OC_InitTypeDef   sConfig[4];
  uint32_t freq;
  void (*p_func[4])(void);
} hwtimer_t;

typedef struct
{
  uint8_t  number;
  uint8_t  index;
  uint32_t active_channel;
} hwtimer_index_t;


static hwtimer_index_t hwtimer_index[TIMER_MAX_CH] = {
    { HWTIMER_TIMER1, HWTIMER_CH1, HAL_TIM_ACTIVE_CHANNEL_1}, // _DEF_TIMER1
    { HWTIMER_TIMER1, HWTIMER_CH2, HAL_TIM_ACTIVE_CHANNEL_2}, // _DEF_TIMER2
    { HWTIMER_TIMER1, HWTIMER_CH3, HAL_TIM_ACTIVE_CHANNEL_3}, // _DEF_TIMER3
    { HWTIMER_TIMER1, HWTIMER_CH4, HAL_TIM_ACTIVE_CHANNEL_4}, // _DEF_TIEMR4
};


static hwtimer_t timer_tbl[HWTIMER_MAX_CH];



void timerInit(void)
{
  timer_tbl[HWTIMER_TIMER1].freq                        = 4000;
  timer_tbl[HWTIMER_TIMER1].hTIM.Instance               = TIM4;
  timer_tbl[HWTIMER_TIMER1].hTIM.Init.Prescaler         = (uint32_t)((SystemCoreClock / 2) / timer_tbl[HWTIMER_TIMER1].freq  ) - 1;   // 4Khz
  timer_tbl[HWTIMER_TIMER1].hTIM.Init.ClockDivision     = 0;
  timer_tbl[HWTIMER_TIMER1].hTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
  timer_tbl[HWTIMER_TIMER1].hTIM.Init.RepetitionCounter = 0;
  timer_tbl[HWTIMER_TIMER1].p_func[0] = NULL;
  timer_tbl[HWTIMER_TIMER1].p_func[1] = NULL;
  timer_tbl[HWTIMER_TIMER1].p_func[2] = NULL;
  timer_tbl[HWTIMER_TIMER1].p_func[3] = NULL;
}

void timerStop(uint8_t channel)
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[hwtimer_index[channel].number];

  HAL_TIM_Base_DeInit(&p_timer->hTIM);
}

void timerSetPeriod(uint8_t channel, uint32_t period_us)
{
  hwtimer_t *p_timer;
  uint32_t period;
  uint32_t time_div;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[hwtimer_index[channel].number];


  time_div = 1000000 / p_timer->freq;

  period = period_us/time_div;

  if (period == 0)
  {
    period = 1;
  }

  p_timer->hTIM.Init.Period = period - 1;
}

void timerAttachInterrupt(uint8_t channel, void (*func)(void))
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[hwtimer_index[channel].number];

  timerStop(channel);
  p_timer->p_func[hwtimer_index[channel].index] = func;
}

void timerDetachInterrupt(uint8_t channel)
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[hwtimer_index[channel].number];

  timerStop(channel);
  p_timer->p_func[hwtimer_index[channel].index] = NULL;
}

void timerStart(uint8_t channel)
{
  hwtimer_t *p_timer;
  uint32_t timer_sub_ch = 0;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[hwtimer_index[channel].number];


  switch(hwtimer_index[channel].index)
  {
    case HWTIMER_CH1:
      timer_sub_ch = TIM_CHANNEL_1;
      break;

    case HWTIMER_CH2:
      timer_sub_ch = TIM_CHANNEL_2;
      break;

    case HWTIMER_CH3:
      timer_sub_ch = TIM_CHANNEL_3;
      break;

    case HWTIMER_CH4:
      timer_sub_ch = TIM_CHANNEL_4;
      break;
  }

  HAL_TIM_OC_Init(&p_timer->hTIM);
  HAL_TIM_OC_ConfigChannel(&p_timer->hTIM,  &p_timer->sConfig[hwtimer_index[channel].index], timer_sub_ch);
  HAL_TIM_OC_Start_IT(&p_timer->hTIM, timer_sub_ch);
}

void timerCallback(TIM_HandleTypeDef *htim)
{
  uint8_t  i;
  uint32_t index;
  hwtimer_t *p_timer;

  for (i=0; i<TIMER_MAX_CH; i++)
  {
    p_timer = &timer_tbl[hwtimer_index[i].number];
    index   = hwtimer_index[i].index;

    if (htim->Channel == hwtimer_index[i].active_channel)
    {
      if( p_timer->p_func[index] != NULL )
      {
        (*p_timer->p_func[index])();
      }
    }
  }
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  timerCallback(htim);
}

void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer_tbl[HWTIMER_TIMER1].hTIM);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == timer_tbl[HWTIMER_TIMER1].hTIM.Instance )
  {
    __HAL_RCC_TIM4_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM4_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == timer_tbl[HWTIMER_TIMER1].hTIM.Instance)
  {
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  }
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == timer_tbl[HWTIMER_TIMER1].hTIM.Instance)
  {
    __HAL_RCC_TIM4_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM4_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  }
}


#endif
