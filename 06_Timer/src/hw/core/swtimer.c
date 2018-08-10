/*
 * swtimer.c
 *
 *  Created on: 2018. 8. 11.
 *      Author: Baram
 */




#include "swtimer.h"


#ifdef _USE_HW_SWTIMER

typedef struct
{

  uint8_t  Timer_En;             // Ÿ�̸� �ο��̺� ��ȣ
  uint8_t  Timer_Mode;           // Ÿ�̸� ���
  uint32_t Timer_Ctn;            // ������ Ÿ�̸� ��
  uint32_t Timer_Init;           // Ÿ�̸� �ʱ�ȭ�ɶ��� ī��Ʈ ��
  void (*TmrFnct)(void *);       // ����ɶ� ����� �Լ�
  void *TmrFnctArg;              // �Լ��� ������ �μ���
} swtimer_t;



//-- Internal Variables
//
static volatile uint32_t sw_timer_counter      = 0;
static volatile uint16_t sw_timer_handle_index = 0;
static swtimer_t  swtimer_tbl[_HW_DEF_SW_TIMER_MAX];           // Ÿ�̸� �迭 ����



//-- External Variables
//


//-- Internal Functions
//



//-- External Functions
//


bool swtimerInit(void)
{
  uint8_t i;
  static uint8_t excute = 0;


  if (excute == 1)
  {
    return false;  // �̹� �ѹ� �����ߴٸ� ����.
  }


  // ����ü �ʱ�ȭ
  for(i=0; i<_HW_DEF_SW_TIMER_MAX; i++)
  {
    swtimer_tbl[i].Timer_En   = OFF;
    swtimer_tbl[i].Timer_Ctn  = 0;
    swtimer_tbl[i].Timer_Init = 0;
    swtimer_tbl[i].TmrFnct    = NULL;
  }

  excute = 1;

  return true;
}

void swtimerISR(void)
{
  uint8_t i;


  sw_timer_counter++;


  for (i=0; i<_HW_DEF_SW_TIMER_MAX && i<sw_timer_handle_index; i++)     // Ÿ�̸� ������ŭ
  {
    if ( swtimer_tbl[i].Timer_En == ON)                         // Ÿ�̸Ӱ� Ȱ��ȭ ���?
    {
      swtimer_tbl[i].Timer_Ctn--;                               // Ÿ�̸Ӱ� ����

      if (swtimer_tbl[i].Timer_Ctn == 0)                        // Ÿ�̸� �����÷ξ�
      {
        if(swtimer_tbl[i].Timer_Mode == ONE_TIME)               // �ѹ��� �����ϴ°Ÿ�
        {
          swtimer_tbl[i].Timer_En = OFF;                        // Ÿ�̸� OFF �Ѵ�.
        }

        swtimer_tbl[i].Timer_Ctn = swtimer_tbl[i].Timer_Init;   // Ÿ�̸� �ʱ�ȭ

        (*swtimer_tbl[i].TmrFnct)(swtimer_tbl[i].TmrFnctArg);   // �Լ� ����
      }
    }
  }

}

void swtimerSet(uint8_t TmrNum, uint32_t TmrData, uint8_t TmrMode, void (*Fnct)(void *),void *arg)
{
  swtimer_tbl[TmrNum].Timer_Mode = TmrMode;    // ��弳��
  swtimer_tbl[TmrNum].TmrFnct    = Fnct;       // ������ �Լ�
  swtimer_tbl[TmrNum].TmrFnctArg = arg;        // �Ű�����
  swtimer_tbl[TmrNum].Timer_Ctn  = TmrData;
  swtimer_tbl[TmrNum].Timer_Init = TmrData;
}

void swtimerStart(uint8_t TmrNum)
{
  if(TmrNum < _HW_DEF_SW_TIMER_MAX)
  {
    swtimer_tbl[TmrNum].Timer_Ctn = swtimer_tbl[TmrNum].Timer_Init;
    swtimer_tbl[TmrNum].Timer_En  = ON;
  }
}

void swtimerStop (uint8_t TmrNum)
{
  if(TmrNum < _HW_DEF_SW_TIMER_MAX)
  {
    swtimer_tbl[TmrNum].Timer_En = OFF;
  }
}

void swtimerReset(uint8_t TmrNum)
{
  swtimer_tbl[TmrNum].Timer_En   = OFF;
  swtimer_tbl[TmrNum].Timer_Ctn  = swtimer_tbl[TmrNum].Timer_Init;
}

swtimer_handle_t swtimerGetHandle(void)
{
  swtimer_handle_t TmrIndex = sw_timer_handle_index;

  sw_timer_handle_index++;

  return TmrIndex;
}

uint32_t swtimerGetCounter(void)
{
  return sw_timer_counter;
}


#endif
