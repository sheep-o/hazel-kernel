#include <syscall.h>
#include <printf.h>

/**
 * Handles system calls by interpreting the values in the provided register frame.
 *
 * @param regs A structure containing the register state which holds the parameters for the system call.
 */
void syscall_handler(struct reg_frame regs) {
    switch (regs.eax) {
        case SYS_WRITE: {
            const uint32_t fd = regs.ebx;
            uint8_t *const buf = (uint8_t *) regs.ecx;
            if (fd == FD_STDOUT)
                printf("%s", buf);
        }
        default:
            break;
    }
}