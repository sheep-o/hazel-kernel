#include <process.h>
#include <exception.h>
#include <gdt.h>
#include <util.h>
#include <kernel.h>
#include <allocator.h>

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

extern struct k_ctx ctx;
extern uint32_t boot_page_dir[1024];
extern uint32_t boot_page_tab[1024];
uint32_t tmp_page_tab[1024];

void *memcpy(void *dest, const void *src, int n) {
    // Cast pointers to unsigned char* for byte-wise operations
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    // Copy bytes one by one
    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

extern struct tss_entry tss;

void process_init(struct process *p, const Elf32_Ehdr *elf, bool change_page_dir) {
    ASSERT(elf->e_ident[EI_MAG0] == ELFMAG0
           && elf->e_ident[EI_MAG1] == ELFMAG1
           && elf->e_ident[EI_MAG2] == ELFMAG2
           && elf->e_ident[EI_MAG3] == ELFMAG3,
           "Executable has an invalid ELF header");

    ASSERT(elf->e_ident[EI_CLASS] == ELFCLASS32, "Executable is not 32-bit");

    uint32_t *page_dir  = alloc_first_page();
    uint32_t *code_tab  = alloc_first_page();
    uint32_t *stack_tab = alloc_first_page();
    const uint32_t stack_bottom = (uint32_t)alloc_first_page();

    // Map kernel
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA)] = ((uint32_t)boot_page_tab - KERNEL_VMA) | 7;

    // Map 4mib of stack space under the kernel
    /*
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA - 1)] = ustack_tab | 7;
    for (int i = 0; i < 1024*PAGE_SIZE; i += PAGE_SIZE)
        stack_tab[PAGE_TAB_INDEX(KERNEL_VMA - STACK_SIZE + i)] = (stack_bottom + i) | 7;
    */
    // Map 4 kib of stack space for now
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA - 1)] = ((uint32_t)stack_tab - KERNEL_VMA) | 7;
    stack_tab[PAGE_TAB_INDEX((KERNEL_VMA - 1) & 0xFFFFF000)] = (stack_bottom - KERNEL_VMA) | 7;

    // Map the load sections
    const Elf32_Phdr *phdr = (Elf32_Phdr *)((uint32_t)elf + elf->e_phoff);
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            const uint32_t s = (uint32_t)alloc_enough(phdr[i].p_filesz);

            // Map the ELF image to its specified entry
            page_dir[PAGE_DIR_INDEX(phdr[i].p_vaddr)] = ((uint32_t)code_tab - KERNEL_VMA) | 7;
            code_tab[PAGE_TAB_INDEX(phdr[i].p_vaddr)] = (s - KERNEL_VMA) | 7;
        }
    }
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t)page_dir - KERNEL_VMA) : "memory");
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            // Map the ELF image to its specified entry
            memcpy((void *)phdr[i].p_vaddr, (uint8_t *)elf + phdr[i].p_offset, phdr[i].p_filesz);
        }
    }

    p->ip = ctx.init_elf->e_entry;
    p->sp = KERNEL_VMA - 0x4;
    p->page_directory = (uint32_t)page_dir - KERNEL_VMA;
    p->next = 0;
    p->state = PROC_OK;

    p->c.esp = KERNEL_VMA - 0x4 - sizeof(struct int_ctx);
    p->c.ebp = KERNEL_VMA - 0x4;

    if (!change_page_dir) {
        // Set up a stack frame so the scheduler can switch to this process
        struct int_ctx ic = {
                .cs = 0x1b,
                .eflags = 0x202,
                .eip = p->ip,
                .esp = p->c.ebp,
                .ss = 0x23,
        };
        uint32_t *stack = (uint32_t *)(p->c.esp);
        memcpy(stack, &ic, sizeof(struct int_ctx));

        asm volatile("mov %0, %%cr3"::"r"((uint32_t) boot_page_dir - KERNEL_VMA) : "memory");
    }
}

uint32_t process_schedule() {
    while (1) {
        ctx.current_process = ctx.current_process->next;
        if (ctx.current_process->state != PROC_ZOMBIE) {
            break;
        }
    }
    asm volatile("mov %0, %%cr3" :: "r"(ctx.current_process->page_directory) : "memory");
    return ctx.current_process->sp;
}
