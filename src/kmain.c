#include <../include/arch/i686/interrupts.h>
#include <timer.h>
#include <../include/arch/i686/uart.h>
#include <../include/arch/i686/user.h>
#include <../include/kernel.h>
#include <../include/arch/i686/allocator.h>
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
