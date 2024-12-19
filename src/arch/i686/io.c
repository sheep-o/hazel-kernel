#include "io.h"

uint8_t inb(uint16_t port) {
    uint8_t result;

    asm volatile ("in %%dx, %%al"
            : "=a" (result)
            : "d"  (port));

    return result;
}

void outb(uint16_t port, uint8_t byte) {
    asm volatile ("out %%al, %%dx"
            : // no output
            : "a" (byte), "d" (port));
}

uint16_t inw(uint16_t port) {
    uint16_t result;

    asm volatile ("in %%dx, %%ax"
            : "=a" (result)
            : "d"  (port));

    return result;
}

void outw(uint16_t port, uint16_t byte) {
    asm volatile ("out %%ax, %%dx"
            : // no output
            : "a" (byte), "d" (port));
}

uint32_t inl(uint16_t port) {
    uint32_t result;

    asm volatile ("in %%dx, %%eax"
            : "=a" (result)
            : "d"  (port));

    return result;
}

void outl(uint16_t port, uint32_t byte) {
    asm volatile ("out %%eax, %%dx"
            : // no output
            : "a" (byte), "d" (port));
}

