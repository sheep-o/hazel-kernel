#include <stdint.h>

void _start(void) {
  asm volatile("int $0x80" : : "a"(3), "d"(3));
  uint8_t *shm = (uint8_t *)0x6000;
  while (1)
      asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(shm), "d"(1));
  /*
  for (int i = 0; i < 1000; i++)
      asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));

  asm volatile("int $0x80" : : "a"(60));
  */
}
