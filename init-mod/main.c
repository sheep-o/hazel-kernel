#include <stdint.h>
const char hello[] = "A";

void _start(void) {
    asm volatile("int $0x80" : : "a"(2));
    uint8_t *shm = (uint8_t *)0x5000;
    shm[0] = '%';
    shm[1] = '\0';
    while (1)
        asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));
    /*
    for (int i = 0; i < 1000; i++)
        asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));

    asm volatile("int $0x80" : : "a"(60));
    */
}