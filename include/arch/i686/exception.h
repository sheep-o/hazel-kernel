#ifndef HAZEL_EXCEPTION_H_
#define HAZEL_EXCEPTION_H_

#include <stdint.h>

/**
 * @brief Represents the CPU's state at the time of the exception
 */
struct ex_frame {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /*  Pushed by pusha. */
    uint32_t n, err;    /*  Interrupt number and error code (if applicable) */

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};

/**
 * @brief Handles CPU exceptions by printing out the CPU state and halting
 * @param r CPU state
 * @todo Allow for recovery from certain exceptions like page faults
 */
void exception_handler(struct ex_frame r);

#endif
