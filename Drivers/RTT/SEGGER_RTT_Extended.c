#include "SEGGER_RTT.h"

#include <string.h>  // for memcpy

static void _TransmitBlocking(SEGGER_RTT_BUFFER_UP* pRing, char* pBuffer, unsigned NumBytes) {
    // memcpy(&temp, pRing, sizeof(SEGGER_RTT_BUFFER_UP));
    //
    // Write data to buffer and handle wrap-around if necessary
    //
    pRing->pBuffer = pBuffer;  // 重定向缓冲区
    pRing->WrOff = 0;          // 复位写入指针
    pRing->RdOff = 0;          // 复位读取指针
    pRing->SizeOfBuffer = NumBytes + 1;
    RTT__DMB();
    pRing->WrOff = NumBytes;
    while (pRing->WrOff > pRing->RdOff) {
    }
    // memcpy(pRing, &temp, sizeof(SEGGER_RTT_BUFFER_UP));
}

void SEGGER_RTT_TransmitNoLock(unsigned BufferIndex, char* pBuffer, unsigned NumBytes) {
    SEGGER_RTT_BUFFER_UP* pRing;
    //
    // Get "to-host" ring buffer.
    //
    while (SEGGER_RTT_HASDATA_UP(BufferIndex))
        ;

    pRing =
        (SEGGER_RTT_BUFFER_UP*)((uintptr_t)&_SEGGER_RTT.aUp[BufferIndex] +
                                SEGGER_RTT_UNCACHED_OFF);  // Access uncached to make sure we see
                                                           // changes made by the J-Link side and
                                                           // all of our changes go into HW directly
                                                           //
                                                           // How we output depends upon the mode...
                                                           //

    _TransmitBlocking(pRing, pBuffer, NumBytes);

    return;
}

void SEGGER_RTT_SetUpBufferWroff(unsigned BufferIndex, unsigned WrOff) {
    SEGGER_RTT_BUFFER_UP* pRing;
    pRing =
        (SEGGER_RTT_BUFFER_UP*)((uintptr_t)&_SEGGER_RTT.aUp[BufferIndex] + SEGGER_RTT_UNCACHED_OFF);
    RTT__DMB();
    pRing->WrOff = WrOff;
}

void SEGGER_RTT_SetUpBufferSize(unsigned BufferIndex, unsigned size) {
    SEGGER_RTT_BUFFER_UP* pRing;
    pRing =
        (SEGGER_RTT_BUFFER_UP*)((uintptr_t)&_SEGGER_RTT.aUp[BufferIndex] + SEGGER_RTT_UNCACHED_OFF);
    RTT__DMB();
    pRing->SizeOfBuffer = size;
}