#include <stdint.h>
#include <interrupts.h>
#include <timer.h>
#include <uart.h>
#include <heap.h>
#include "kernel.h"

struct k_ctx ctx = {};

void kmain(void) {
    tmr_init();
    int_init();
    uart_init();
    heap_init();

    uart_puts("Hello, world!\n");

    while (1);
}
