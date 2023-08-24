
#include "string.h"
#include "gd32f3x0.h"
#include "systick.h"
#include "main.h"
#include "log.h"
#include "uart.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Extended.h"
#include "scanf.h"
#include "test.h"
extern void DMA_Channel1_2_IRQHandler(void);
extern void USART1_IRQHandler(void);
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void) {
    systick_config();

    Usartx_Init();
    SEGGER_RTT_Init();

    uart_test();
}
