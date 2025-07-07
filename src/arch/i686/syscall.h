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

void syscall_read(uint32_t fd, uint8_t *buf, uint32_t count);
void syscall_write(uint32_t fd, uint8_t *buf, uint32_t count);
void syscall_create_shm();
void syscall_open_shm(uint32_t pid);
void syscall_exit(uint32_t status);

#define FD_STDIN 0
#define FD_STDOUT 1

#define SHM_VADDR_CREATED 0x5000
#define SHM_VADDR_OPENED 0x6000

struct reg_frame {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /*  Pushed by pusha. */
};

uint32_t syscall_handler(struct reg_frame regs);

#endif
