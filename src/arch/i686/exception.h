#ifndef HAZEL_EXCEPTION_H_
#define HAZEL_EXCEPTION_H_

#include <stdint.h>

struct ex_frame {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /*  Pushed by pusha. */
    uint32_t n, err;    /*  Interrupt number and error code (if applicable) */

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};

void exception_handler(struct ex_frame r);

#endif
