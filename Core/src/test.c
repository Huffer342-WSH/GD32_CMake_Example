#ifndef _TEST_H_
#define _TEST_H_

#include "test.h"
#include "systick.h"
#include "uart.h"
#include "log.h"
#include "string.h"
#include "SEGGER_RTT.h"

static char str0[] = "Uart test\r\n";
static char str1[] = "Control uart transmission by register mode\r\n";
static char str2[] = "Send data from uart by printf function\r\n";
static char buf[128];

void RTT_test() {
    static uint8_t init_flag = 0;
    // 初始化SEGGER RTT
    if (!init_flag) {
        SEGGER_RTT_Init();
        init_flag = 1;
    }

    SEGGER_RTT_printf(0, "Here is Segger RTT default channel\r\n");
    for (int i = 0; i < 5; i++) {
        delay_ms(1000);
        SEGGER_RTT_printf(0, "Second %d\r\n", i);
    }
}

void uart_test() {
    static uint8_t init_flag = 0;
    // 初始化uart
    if (!init_flag) {
        Usartx_Init();
        init_flag = 1;
    }

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
            if (rx_size >= 8 && !memcmp(rx_str, "RTT test", 8)) {
                LOG("RTT_test start.");
                RTT_test();
                LOG("RTT_test end.");
            }
        }
    }
}

#endif /* _TEST_H_ */