#include <io.h>

uint8_t hzl::inb(uint16_t port) {
  uint8_t result;

  asm volatile ("in %%dx, %%al"
          : "=a" (result)
          : "d"  (port));

  return result;
}

void hzl::outb(uint16_t port, uint8_t byte) {
  asm volatile ("out %%al, %%dx"
          : // no output
          : "a" (byte), "d" (port));
}

uint16_t hzl::inw(uint16_t port) {
  uint16_t result;

  asm volatile ("in %%dx, %%ax"
          : "=a" (result)
          : "d"  (port));

  return result;
}

void hzl::outw(uint16_t port, uint16_t byte) {
  asm volatile ("out %%ax, %%dx"
          : // no output
          : "a" (byte), "d" (port));
}

uint32_t hzl::inl(uint16_t port) {
  uint32_t result;

  asm volatile ("in %%dx, %%eax"
          : "=a" (result)
          : "d"  (port));

  return result;
}

void hzl::outl(uint16_t port, uint32_t byte) {
  asm volatile ("out %%eax, %%dx"
          : // no output
          : "a" (byte), "d" (port));
}