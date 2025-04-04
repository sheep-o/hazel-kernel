#ifndef HAZEL_UART_H_
#define HAZEL_UART_H_

#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

#define IS_TRANSMIT_EMPTY(x) (inb(x + 5) & 0x20)

uint8_t uart_init(void);
void uart_putc(const char c);
void uart_puts(const char *s);

#endif
