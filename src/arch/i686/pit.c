#include "timer.h"
#include "io.h"
#include "pic.h"
#include <stdint.h>

void tmr_init(void) {
    outb(PIT_MODE_CMD, PIT_CHAN0 | PIT_LOHIGH | PIT_MODE2);
    outb(PIT_CHAN0_DATA, (uint8_t)(DIVISOR & 0xff));
    outb(PIT_CHAN0_DATA, (uint8_t)((DIVISOR & 0xff00) >> 8));
}

int tmr_pit_handler(void) {
    outb(PIT_CHAN0_DATA, (uint8_t)(DIVISOR & 0xff));
    outb(PIT_CHAN0_DATA, (uint8_t)((DIVISOR & 0xff00) >> 8));
    pic_send_eoi(0);

    return 0;
}
