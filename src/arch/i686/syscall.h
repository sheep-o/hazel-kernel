#ifndef HAZEL_SYSCALL_H
#define HAZEL_SYSCALL_H

#include <stdint.h>

enum SYSCALL {
    SYS_READ,
    SYS_WRITE,
};

#define FD_STDIN 0
#define FD_STDOUT 1

struct reg_frame {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /*  Pushed by pusha. */
};

void syscall_handler(struct reg_frame regs);

#endif
