#ifndef _SEGGER_RTT_EXTENDED_H_
#define _SEGGER_RTT_EXTENDED_H_

void SEGGER_RTT_TransmitNoLock(unsigned BufferIndex, char* pBuffer, unsigned NumBytes);
void SEGGER_RTT_SetUpBufferWroff(unsigned BufferIndex, unsigned WrOff);
void SEGGER_RTT_SetUpBufferSize(unsigned BufferIndex, unsigned size);
#endif /* _SEGGER_RTT_EXTENDED_H_ */