/*
 * uart.c
 *
 *  Created on: 2018. 8. 10.
 *      Author: Baram
 */





#include "uart.h"
#include "core/qbuffer.h"
#include "usb_cdc/vcp.h"


#ifdef _USE_HW_UART




#define UART_MODE_POLLING       0
#define UART_MODE_INTERRUPT     1
#define UART_MODE_DMA           2
#define UART_MODE_VCP           3


#define UART_RX_BUF_LENGTH      16
#define UART_RX_QBUF_LENGTH     256



typedef struct
{
  bool     is_open;
  uint32_t baud;
  bool     vcp_mode;
  uint8_t  tx_mode;
  uint8_t  rx_mode;

  uint8_t  rx_buf[UART_RX_BUF_LENGTH];

  qbuffer_t qbuffer_rx;

  DMA_HandleTypeDef  hdma_rx;
  UART_HandleTypeDef handle;
} uart_t;



uart_t   uart_tbl[UART_MAX_CH];
uint8_t  uart_rx_qbuf[UART_MAX_CH][UART_RX_BUF_LENGTH];



void uartStartRx(uint8_t channel);
void uartRxHandler(uint8_t channel);



void uartInit(void)
{
  uint8_t i;


  for (i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open  = false;
    uart_tbl[i].vcp_mode = false;
    uart_tbl[i].rx_mode  = UART_MODE_POLLING;
    uart_tbl[i].tx_mode  = UART_MODE_POLLING;
  }
}

bool uartOpen(uint8_t channel, uint32_t baud)
{
  bool ret = false;
  uart_t *p_uart;

  switch(channel)
  {
    case _DEF_UART1:
      p_uart = &uart_tbl[channel];

      p_uart->vcp_mode = true;
      p_uart->baud     = baud;
      p_uart->is_open  = true;

      p_uart->rx_mode  = UART_MODE_VCP;
      p_uart->tx_mode  = UART_MODE_VCP;

      uartStartRx(channel);
      break;
  }

  return ret;
}

void uartStartRx(uint8_t channel)
{
  uart_t *p_uart = &uart_tbl[channel];

  if (p_uart->rx_mode == UART_MODE_INTERRUPT)
  {
    HAL_UART_Receive_IT(&p_uart->handle, p_uart->rx_buf, 1);
  }
  if (p_uart->rx_mode == UART_MODE_DMA)
  {
    HAL_UART_Receive_DMA(&p_uart->handle, (uint8_t *)p_uart->qbuffer_rx.p_buf, p_uart->qbuffer_rx.length);
  }
}

bool uartClose(uint8_t channel)
{
  bool ret = false;


  if (channel >= UART_MAX_CH)
  {
    return false;
  }


  if (uart_tbl[channel].is_open == true && uart_tbl[channel].vcp_mode == false)
  {
    if(HAL_UART_DeInit(&uart_tbl[channel].handle) == HAL_OK)
    {
      ret = true;
    }
  }


  return ret;
}

uint32_t uartAvailable(uint8_t channel)
{
  uint32_t ret;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return 0;
  }

  if (p_uart->rx_mode == UART_MODE_VCP)
  {
    ret = vcpAvailable();
  }
  if (p_uart->rx_mode == UART_MODE_INTERRUPT)
  {
    ret = qbufferAvailable(&uart_tbl[channel].qbuffer_rx);
  }
  if (p_uart->rx_mode == UART_MODE_DMA)
  {
    p_uart->qbuffer_rx.ptr_in = p_uart->qbuffer_rx.length - p_uart->hdma_rx.Instance->NDTR;
    ret = qbufferAvailable(&p_uart->qbuffer_rx);
  }

  return ret;
}

void uartFlush(uint8_t channel)
{
  if (uart_tbl[channel].rx_mode == UART_MODE_VCP)
  {
    vcpFlush();
  }
  if (uart_tbl[channel].rx_mode == UART_MODE_INTERRUPT)
  {
    qbufferFlush(&uart_tbl[channel].qbuffer_rx);
  }
  if (uart_tbl[channel].rx_mode == UART_MODE_DMA)
  {
    uart_tbl[channel].qbuffer_rx.ptr_in  = uart_tbl[channel].qbuffer_rx.length - uart_tbl[channel].hdma_rx.Instance->NDTR;
    uart_tbl[channel].qbuffer_rx.ptr_out = uart_tbl[channel].qbuffer_rx.ptr_in;
  }
}

void uartPutch(uint8_t channel, uint8_t ch)
{
  uartWrite(channel, &ch, 1 );
}

uint8_t uartGetch(uint8_t channel)
{
  uint8_t ret = 0;


  while(1)
  {
    if (uartAvailable(channel) > 0)
    {
      ret = uartRead(channel);
      break;
    }
  }

  return ret;
}

int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];


  if (p_uart->tx_mode == UART_MODE_VCP)
  {
    vcpWrite(p_data, length);
  }
  if (p_uart->tx_mode == UART_MODE_POLLING)
  {
    if (HAL_UART_Transmit(&p_uart->handle, (uint8_t*)p_data, length, 1000) == HAL_OK)
    {
      ret = length;
    }
  }

  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];


  if (p_uart->rx_mode == UART_MODE_VCP)
  {
    ret = vcpRead();
  }
  if (p_uart->rx_mode == UART_MODE_INTERRUPT)
  {
    qbufferRead(&p_uart->qbuffer_rx, &ret, 1);
  }
  if (p_uart->rx_mode == UART_MODE_DMA)
  {
    qbufferRead(&p_uart->qbuffer_rx, &ret, 1);
  }


  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  ret = uartWrite(channel, (uint8_t *)print_buffer, len);

  return ret;
}

void uartRxHandler(uint8_t channel)
{
  uart_t *p_uart = &uart_tbl[channel];


  if(p_uart->rx_mode == UART_MODE_INTERRUPT)
  {
      qbufferWrite(&p_uart->qbuffer_rx, &p_uart->rx_buf[0], 1);

      __HAL_UNLOCK(&p_uart->handle);
      uartStartRx(channel);
  }
}

void uartErrHandler(uint8_t channel)
{
  uartFlush(channel);
  uartStartRx(channel);
}



#endif
