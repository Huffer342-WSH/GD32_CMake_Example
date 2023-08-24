/**

 * @brief 将printf定位到串口
 *		三选一，将不需要的注释掉
 *
 *   ===================选项================
 *   | gcc for arm  	<==> GCC           |
 *   | mdk标准库		    <==> MDK_STD       |
 *   |  mdk micro lib   <==> MDL_MICROLIB  |
 */
#define MPALAND_PRINTF
// #define GCC
// #define MDK_STD
// #define MDL_MICROLIB

#ifdef MPALAND_PRINTF
#include "gd32f3x0.h"
void _putchar(char character) {
    while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
        ;
    USART_TDATA(USART1) = (uint32_t)character;
}
#endif
#ifdef GCC
#include "gd32f3x0.h"

int _write(int fd, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
            ;
        USART_TDATA(USART1) = (uint32_t)*ptr;
        ptr++;
    }
    while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
        ;
    return len;
}

#endif

#ifdef MDK_STD

/* 告知连接器不从C库链接使用半主机的函数 */
#pragma import(__use_no_semihosting)

/* 定义 _sys_exit() 以避免使用半主机模式 */
void _sys_exit(int x) { x = x; }

/* 标准库需要的支持类型 */
struct __FILE {
    int handle;
};

FILE __stdout;

/*  */
int fputc(int ch, FILE *stream) {
    /* 堵塞判断串口是否发送完成 */
    while ((USART1->SR & 0X40) == 0)
        ;

    /* 串口发送完成，将该字符发送 */
    USART1->DR = (uint8_t)ch;

    return ch;
}
#endif

#ifdef MDL_MICROLIB
int fputc(int ch, FILE *stream) {
    /* 堵塞判断串口是否发送完成 */
    while ((USART1->SR & 0X40) == 0)
        ;

    /* 串口发送完成，将该字符发送 */
    USART1->DR = (uint8_t)ch;

    return ch;
}

#endif
