#ifndef HAZEL_UTIL_H
#define HAZEL_UTIL_H

#include <uart.h>
#include <printf.h>

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            hzl::uart_init(); \
            printf("Assertion failed: %s\nFile: %s\nLine: %d\nFunction: %s\n", \
                    message, __FILE__, __LINE__, __func__); \
            while (1); \
        } \
    } while (0)

#endif
