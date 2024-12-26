#include <stdint.h>
#include <interrupts.h>
#include <timer.h>
#include <uart.h>
#include <heap.h>
#include "kernel.h"
#include "printf.h"

struct k_ctx ctx = {};

void kmain(void) {
    tmr_init();
    int_init();
    uart_init();
    heap_init();

    void *test = heap_alloc(0x100000);
    printf("1 MiB allocated at 0x%08X\n", test);

    while (1);
}
