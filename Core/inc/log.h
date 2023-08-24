#ifndef _LOG_H_
#define _LOG_H_

#include "printf.h"
#ifdef DEBUG
#define LOG(format, ...) printf(format "\r\n", ##__VA_ARGS__)
#else
#define LOG(format, ...)
#endif

#ifdef DEBUG
#define INFO(format, ...) printf("[INFO]" format "\r\n", ##__VA_ARGS__)
#else
#define INFO(format, ...)
#endif

#ifdef DEBUG
#define ERROR(format, ...)                                                                      \
    printf("\e[0;31m[ERROR] " format "(%-6s:%d->%s)\e[0m\r\n"##__VA_ARGS__, __FILE__, __LINE__, \
           __func__)
#else
#define ERROR(format, ...)
#endif
#endif /* _LOG_H_ */
