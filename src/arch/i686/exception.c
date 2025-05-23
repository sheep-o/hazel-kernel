#include <exception.h>
#include <printf.h>

/**
 * Handles CPU exceptions by printing the state of the registers and halting execution.
 *
 * This function is called whenever a CPU exception occurs.
 *
 * @note This function is not meant to be called outside of an ISR stub
 *
 * TODO: Allow recovery from certain exceptions, such as a page fault
 *
 * @param r The CPU state at the time of the exception.
 */
void exception_handler(struct ex_frame r) {
    printf("Exception 0x%X (0x%X) has occurred\nEAX: 0x%08X\nECX: 0x%08X\nEDX: 0x%08X\nEBX: 0x%08X\nESI: 0x%08X\nEDI: 0x%08X\nEBP: 0x%08X\nESP: 0x%08X\n",
            r.n, r.err, r.eax, r.ecx, r.edx, r.ebx, r.esi, r.edi, r.ebp, r.esp);
    printf("\nEIP:     0x%08X\nCS:      0x%08X\nEFLAGS:  0x%08X\n",
            r.eip, r.cs, r.eflags);

    uint32_t cr2 = 0;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2));
    printf("\nCR2: 0x%08X\n", cr2);

    while (1);
}
