#include <stdint.h>
#include <interrupts.h>
#include <timer.h>
#include <uart.h>

void kmain(void) {
    tmr_init();
    int_init();
    uart_init();

    uart_puts("Hello, world!\n");

    while (1);
}
