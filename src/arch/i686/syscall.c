#include <syscall.h>
#include <printf.h>
#include <kernel.h>
#include <allocator.h>
#include <util.h>

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
            ASSERT(ctx.proc_count > 1, "Attempt to exit init process");
            --ctx.proc_count;
            return 1;
        }
        case SYS_CREATE_SHM: {
            ASSERT(ctx.current_process->shm == 0, "Can only support 1 shared memory page rn");
            uint8_t *page = alloc_first_page() - KERNEL_VMA;
            ctx.current_process->shm = page;
            process_map(ctx.current_process, page, 0x5000);
            regs.eax = 0x5000;
            break;
        }
        case SYS_OPEN_SHM: {
            const uint32_t pid = regs.edx;
            bool found = false;
            const struct process *p = ctx.current_process;
            do {
                if (p->id == pid) {
                    found = true;
                    break;
                }
                p = p->next;
            } while (p != ctx.current_process);
            ASSERT(found, "Failed to find a process with the given PID");
            process_map(ctx.current_process, p->shm, 0x6000);
            break;
        }
        default:
            break;
    }

    return 0;
}