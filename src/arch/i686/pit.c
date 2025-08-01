#include <timer.h>
#include <io.h>
#include <pic.h>
#include <stdint.h>
#include <kernel.h>

/**
 * @brief Initializes the timer module (Programmable Interval Timer).
 *
 * This function configures the Programmable Interval Timer (PIT) to operate in mode 2,
 * which is a periodic mode. It sends the necessary commands and data to the PIT
 * to configure the timer channel 0 for the specified frequency derived from the
 * provided divisor.
 *
 * The divisor determines the frequency of interrupts generated by the timer.
 *
 * @note This function interacts with system hardware and should only be called
 *       during system initialization.
 */
void tmr_init(void) {
    outb(PIT_MODE_CMD, PIT_CHAN0 | PIT_LOHIGH | PIT_MODE2);
    outb(PIT_CHAN0_DATA, DIVISOR & 0xff);
    outb(PIT_CHAN0_DATA, (DIVISOR & 0xff00) >> 8);
}

extern struct k_ctx ctx;

/**
 * @brief Handles the Programmable Interval Timer (PIT) interrupt and determines whether a context switch should occur.
 *
 * @return Returns 1 if a context switch should occur, otherwise 0.
 */
bool tmr_pit_handler(void) {
    outb(PIT_CHAN0_DATA, DIVISOR & 0xff);
    outb(PIT_CHAN0_DATA, (DIVISOR & 0xff00) >> 8);
    pic_send_eoi(0);

    ctx.ticks++;

    if (ctx.current_process && ctx.current_process->next && (ctx.ticks % TICKS_PER_PROCESS) == 0)
        return 1;
    else
        return 0;
}

