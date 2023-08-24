/*!
    \file    systick.c
    \brief   the systick configuration file

    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
    \version 2020-09-30, V2.1.0, firmware for GD32F3x0
    \version 2022-01-06, V2.2.0, firmware for GD32F3x0
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f3x0.h"
#include "systick.h"

volatile static uint32_t systick_31_24;
volatile static uint64_t systick_64_32;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void) {
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(0xFFFFFFUL)) {
        /* capture error */
        while (1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_ms(uint32_t count) {
    for (int i = 0; i < count; i++) {
        delay_tick(SystemCoreClock / 1000 - 3);
    }
}

void delay_us(uint32_t count) {
    for (int i = 0; i < count; i++) {
        delay_tick(SystemCoreClock / 1000000 - 3);
    }
}

void delay_tick(uint32_t delay) {
    uint32_t start_tick = systick_31_24 + (~SysTick->VAL);
    while (Get_Tick() - start_tick < delay) {
    }
}
/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_high_carry(void) {
    systick_31_24 += 0x01000000;
    if (systick_31_24) {
        systick_64_32++;
    }
}

uint32_t Get_Tick(void) { return systick_31_24 + (~SysTick->VAL); }
uint64_t Get_Tick64(void) {
    return systick_64_32 + (uint64_t)systick_31_24 + (uint64_t)(~SysTick->VAL);
}
