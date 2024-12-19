#include <stdint.h>
#include <interrupts.h>
#include <timer.h>

void kmain(void) {
    tmr_init();
    int_init();

    while (1);
}
