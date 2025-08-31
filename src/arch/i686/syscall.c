#include <../../../include/arch/i686/syscall.h>
#include <printf.h>
#include <../../../include/kernel.h>
#include <../../../include/arch/i686/allocator.h>
#include <util.h>

extern struct k_ctx ctx;

/* TODO
void syscall_read(uint32_t fd, uint8_t *buf, uint32_t count) {
}
*/

void syscall_write(uint32_t fd, uint8_t *buf, uint32_t count) {
    (void)count; /* TODO: Use the count argument */
    ASSERT(fd == FD_STDOUT, "Can only write to stdout right now");
    printf("%s", buf);
}

void syscall_create_shm() {
    ASSERT(ctx.current_process->shm == 0, "Can only support 1 shared memory page right now");
    uint8_t *page = alloc_first_page() - KERNEL_VMA;
    ctx.current_process->shm = page;
    process_map(ctx.current_process, page, SHM_VADDR_CREATED);
}

void syscall_open_shm(uint32_t pid) {
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
    process_map(ctx.current_process, p->shm, SHM_VADDR_OPENED);
}

void syscall_exit(uint32_t status) {
    (void)status; /* TODO: Use exit code */
    ASSERT(ctx.proc_count > 1, "Attempt to exit init process");
    --ctx.proc_count;
}

/**
 * Handles system calls by interpreting the values in the provided register frame.
 *
 * @param regs A structure containing the register state which holds the parameters for the system call.
 */
uint32_t syscall_handler(struct reg_frame regs) {
    switch (regs.eax) {
        case SYS_WRITE: {
            const uint32_t fd = regs.ebx;
            const uint32_t count = regs.edx;
            uint8_t *buf = (uint8_t *) regs.ecx;
            syscall_write(fd, buf, count);
            break;
        }
        case SYS_EXIT: {
            const uint32_t status = regs.edi;
            syscall_exit(status);
            return 1;
        }
        case SYS_CREATE_SHM: {
            syscall_create_shm();
            break;
        }
        case SYS_OPEN_SHM: {
            const uint32_t pid = regs.edx;
            syscall_open_shm(pid);
            break;
        }
        default: {
            ASSERT(true, "Invalid system call number");
        }
    }

    return 0;
}