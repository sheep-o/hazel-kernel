#include <interrupts.h>
#include <timer.h>
#include <uart.h>
#include <heap.h>
#include <user.h>
#include <kernel.h>

struct k_ctx ctx = {};

/**
 * @brief Entry point for the kernel.
 *
 * The `kmain` function acts as the primary entry point for the kernel's
 * execution flow after the system has been bootstrapped. It initializes
 * key subsystems required for kernel operation and enters an infinite
 * loop to maintain system control.
 *
 * The function performs the following steps:
 * 1. Initializes the system timer
 * 2. Configures and initializes interrupts
 * 3. Sets up the UART for communication
 * 4. Initializes the memory heap for dynamic memory allocation
 * 5. Calls `user_init` to switch to user-mode
 * 6. Enters an infinite loop
 *
 * @note This function should never return
 */
void kmain(void) {
    tmr_init();
    int_init();
    uart_init();
    heap_init();
    user_init();

    while (1);
}
