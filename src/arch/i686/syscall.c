#include <syscall.h>
#include <printf.h>
#include <kernel.h>

extern struct k_ctx ctx;

/**
 * Handles system calls by interpreting the values in the provided register frame.
 *
 * @param regs A structure containing the register state which holds the parameters for the system call.
 */
uint32_t syscall_handler(struct reg_frame regs) {
    switch (regs.eax) {
        case SYS_WRITE: {
            const uint32_t fd = regs.ebx;
            uint8_t *const buf = (uint8_t *) regs.ecx;
            if (fd == FD_STDOUT)
                printf("%s", buf);

            break;
        }
        case SYS_EXIT: {
            //const uint32_t exit_code = regs.edi;
            return 1;
            break;
        }
        default:
            break;
    }

    return 0;
}