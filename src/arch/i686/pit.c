#include "timer.h"
#include "io.h"
#include "pic.h"
#include <stdint.h>
#include "../../kernel.h"

void tmr_init(void) {
    outb(PIT_MODE_CMD, PIT_CHAN0 | PIT_LOHIGH | PIT_MODE2);
    outb(PIT_CHAN0_DATA, (uint8_t)(DIVISOR & 0xff));
    outb(PIT_CHAN0_DATA, (uint8_t)((DIVISOR & 0xff00) >> 8));
}

extern struct k_ctx ctx;
int tmr_pit_handler(void) {
    outb(PIT_CHAN0_DATA, (uint8_t)(DIVISOR & 0xff));
    outb(PIT_CHAN0_DATA, (uint8_t)((DIVISOR & 0xff00) >> 8));
    pic_send_eoi(0);

    ctx.ticks++;

    return 0;
}

