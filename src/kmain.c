#include <interrupts.h>
#include <timer.h>
#include <uart.h>
#include <user.h>
#include <kernel.h>
#include <allocator.h>
#include <printf.h>


struct k_ctx ctx;

/**
 * @brief Entry point for the kernel.
 * @note This function should never return
 */
void kmain(void) {
    tmr_init();
    int_init();
    uart_init();
#ifdef X86
    extern void vga_clear(void);
    vga_clear();
#endif
    alloc_init();
    user_init();

    while (1);
}
