#include <process.h>
#include <exception.h>
#include <gdt.h>
#include <util.h>
#include <kernel.h>

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

    /*
    const uint32_t upage_dir = (uint32_t)heap_alloc(PAGE_SIZE, true);
    const uint32_t ucode_tab = (uint32_t)heap_alloc(PAGE_SIZE, true);
    const uint32_t ustack_tab = (uint32_t)heap_alloc(PAGE_SIZE, true);
    const uint32_t stack_bottom = (uint32_t)heap_alloc(STACK_SIZE, true);
    */
    const uint32_t upage_dir = 0;
    const uint32_t ucode_tab = 0;
    const uint32_t ustack_tab = 0;
    const uint32_t stack_bottom = 0;

    // Map page directory to 0x1000
    boot_page_dir[PAGE_DIR_INDEX(0x1000)] = ((uint32_t)&tmp_page_tab - KERNEL_VMA) | 7;
    tmp_page_tab[PAGE_TAB_INDEX(0x1000)] = upage_dir | 7;
    // Map code table to 0x2000
    tmp_page_tab[PAGE_TAB_INDEX(0x2000)] = ucode_tab | 7;
    // Map stack table to 0x3000
    tmp_page_tab[PAGE_TAB_INDEX(0x3000)] = ustack_tab | 7;

    uint32_t *const page_dir = (uint32_t *)0x1000;
    uint32_t *const code_tab = (uint32_t *)0x2000;
    uint32_t *const stack_tab = (uint32_t *)0x3000;

    // Map kernel
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA)] = ((uint32_t)boot_page_tab - KERNEL_VMA) | 7;

    // Map 4mib of stack space under the kernel
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA - 1)] = ustack_tab | 7;
    for (int i = 0; i < 1024*PAGE_SIZE; i += PAGE_SIZE)
        stack_tab[PAGE_TAB_INDEX(KERNEL_VMA - STACK_SIZE + i)] = (stack_bottom + i) | 7;

    // Map the load sections
    const Elf32_Phdr *phdr = (Elf32_Phdr *)((uint32_t)elf + elf->e_phoff);
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            //const uint32_t s = (uint32_t)heap_alloc(phdr[i].p_filesz, true);
            const uint32_t s = 0;

            // Map the ELF image to its specified entry
            page_dir[PAGE_DIR_INDEX(phdr[i].p_vaddr)] = ucode_tab | 7;
            code_tab[PAGE_TAB_INDEX(phdr[i].p_vaddr)] = s | 7;
        }
    }
    asm volatile("mov %0, %%cr3" :: "r"(upage_dir) : "memory");
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            // Map the ELF image to its specified entry
            memcpy((void *)phdr[i].p_vaddr, (uint8_t *)elf + phdr[i].p_offset, phdr[i].p_filesz);
        }
    }

    p->ip = ctx.init_elf->e_entry;
    p->sp = KERNEL_VMA - 0x4;
    p->page_directory = upage_dir;
    p->next = 0;

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
    ctx.current_process = ctx.current_process->next;
    asm volatile("mov %0, %%cr3" :: "r"(ctx.current_process->page_directory) : "memory");
    return ctx.current_process->sp;
}
