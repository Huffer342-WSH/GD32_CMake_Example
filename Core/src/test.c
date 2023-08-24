#ifndef _TEST_H_
#define _TEST_H_

#include "test.h"
#include "uart.h"
#include "log.h"
static char str0[] = "Uart test\r\n";
static char str1[] = "Control uart transmission by register mode\r\n";
static char str2[] = "Send data from uart by printf function\r\n";
static char buf[128];
void uart_test() {
    // 初始化uart
    Usartx_Init();

    LOG("%s", str0);

    for (int i = 0; i < sizeof(str1) - 1; i++) {
        while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
            ;
        USART_TDATA(USART1) = (uint32_t)str1[i];
    }

    printf("%s", str2);
    ERROR("error output test");

    LOG("\r\nSerial port receiving test, sending messages to the serial port of the "
        "microcontroller, "
        "and the serial port returns messages.");
    LOG("\r\nThe format of the message returned by the serial port is as follows:");
    LOG("\r\nuart received message:\r\n%s\r\n----- END -----", "[The message you send]");
    UART_Receive_DMA(buf, 128);
    char* rx_str;
    uint32_t rx_size;
    while (1) {
        if (!USART1_Get_Rx(&rx_str, &rx_size)) {
            LOG("uart received message:\r\n%s\r\n----- END -----", rx_str);
        }
    }
}
#endif /* _TEST_H_ */