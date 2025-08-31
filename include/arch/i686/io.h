#ifndef HAZEL_IO_H_
#define HAZEL_IO_H_

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t byte);

uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t byte);

uint32_t inl(uint16_t port);
void outl(uint16_t port, uint32_t byte);

#endif
