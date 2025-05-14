#ifndef HAZEL_SYSCALL_H
#define HAZEL_SYSCALL_H

#include <stdint.h>

enum SYSCALL {
    SYS_READ,
    SYS_WRITE,
    SYS_CREATE_SHM,
    SYS_OPEN_SHM,
    SYS_EXIT = 60,
};

#define FD_STDIN 0
#define FD_STDOUT 1

struct reg_frame {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /*  Pushed by pusha. */
};

uint32_t syscall_handler(struct reg_frame regs);

#endif
