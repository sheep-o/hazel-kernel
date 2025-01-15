#include "syscall.h"
#include "printf.h"

void syscall_handler(struct reg_frame regs) {
    switch (regs.eax) {
        case SYS_WRITE: {
            uint32_t fd = regs.ebx;
            uint8_t *buf = (uint8_t *) regs.ecx;
            if (fd == FD_STDOUT)
                printf("%s", buf);
        }
        default:
            break;
    }
}