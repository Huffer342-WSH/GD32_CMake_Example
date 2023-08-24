#ifndef _UART_H_
#define _UART_H_
#include "gd32f3x0.h"

#define USART1_DMA_TX_CHx DMA_CH3
#define USART1_DMA_RX_CHx DMA_CH4

void Usartx_Init();
void UART_Transmit_DMA(uint32_t usart_periph, uint32_t p_data, uint32_t data_Size);
void UART_Receive_DMA(void* buf, uint32_t size);
uint8_t USART1_Get_Rx(char** addr, uint32_t* size);
#endif /*_UART_H_*/