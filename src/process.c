#include <process.h>
#include <exception.h>
#include <gdt.h>
#include <util.h>
#include <kernel.h>
#include <allocator.h>

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

#define PAGE_TAB_LEN 1024
#define PAGE_DIR_LEN 1024

#define PRESENT (1 << 0)
#define WRITABLE (1 << 1)
#define USER (1 << 2)
#define FULL_ACCESS (PRESENT | WRITABLE | USER)

extern struct k_ctx ctx;
extern struct tss_entry tss;
extern uint32_t boot_page_dir[PAGE_DIR_LEN];
extern uint32_t boot_page_tab[PAGE_TAB_LEN];

void *memcpy(void *dest, const void *src, int n) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

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
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA)] = ((uint32_t)boot_page_tab - KERNEL_VMA) | FULL_ACCESS;

    // Map 4 kib of stack space for now
    page_dir[PAGE_DIR_INDEX(KERNEL_VMA - 1)] = ((uint32_t)stack_tab - KERNEL_VMA) | FULL_ACCESS;
    stack_tab[PAGE_TAB_INDEX((KERNEL_VMA - 1) & PAGE_MASK)] = (stack_bottom - KERNEL_VMA) | FULL_ACCESS;

    // Map the load sections
    const Elf32_Phdr *phdr = (Elf32_Phdr *)((uint32_t)elf + elf->e_phoff);
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            const uint32_t s = (uint32_t)alloc_enough(phdr[i].p_filesz);

            // Map the ELF image to its specified entry
            page_dir[PAGE_DIR_INDEX(phdr[i].p_vaddr)] = ((uint32_t)code_tab - KERNEL_VMA) | FULL_ACCESS;
            code_tab[PAGE_TAB_INDEX(phdr[i].p_vaddr)] = (s - KERNEL_VMA) | FULL_ACCESS;
        }
    }
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t)page_dir - KERNEL_VMA) : "memory");
    for (int i = 0; i < elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            // Map the ELF image to its specified entry
            memcpy((void *) phdr[i].p_vaddr,
                   (uint8_t *) elf + phdr[i].p_offset,
                   phdr[i].p_filesz);
        }
    }

    p->ip = ctx.init_elf->e_entry;
    p->sp = KERNEL_VMA - 0x4; // Set stack right below the kernel
    p->page_directory = (uint32_t)page_dir - KERNEL_VMA;
    p->next = 0;
    p->state = PROC_OK;
    p->shm = 0;
    p->id = ++ctx.proc_count;

    p->c.esp = KERNEL_VMA - 0x4 - sizeof(struct int_ctx);
    p->c.ebp = KERNEL_VMA - 0x4;

    if (!change_page_dir) {
        // Set up a stack frame so the scheduler can switch to this process
        struct int_ctx ic = {
                .cs = USER_CODE_SEG,
                .eflags = 0x202, // Random flags value lol
                .eip = p->ip,
                .esp = p->c.ebp,
                .ss = USER_DATA_SEG,
        };
        uint32_t *stack = (uint32_t *)(p->c.esp);
        memcpy(stack, &ic, sizeof(struct int_ctx));

        asm volatile("mov %0, %%cr3"::"r"((uint32_t) boot_page_dir - KERNEL_VMA)
            : "memory");
    }
}

void process_map(const struct process* p, const void *data, uint32_t addr) {
    uint32_t *page_dir = (uint32_t *)(KERNEL_VMA + p->page_directory);
    if (page_dir[PAGE_DIR_INDEX(addr)] & 1) { // entry is present
        uint32_t *page_tab = (uint32_t *)(KERNEL_VMA + (page_dir[PAGE_DIR_INDEX(addr)] & PAGE_MASK));
        page_tab[PAGE_TAB_INDEX(addr)] = ((uint32_t)data & PAGE_MASK) | FULL_ACCESS;
    } else {
        uint32_t *page_tab = alloc_first_page();
        page_dir[PAGE_DIR_INDEX(addr)] = ((uint32_t)page_tab - KERNEL_VMA) | FULL_ACCESS;
        page_tab[PAGE_TAB_INDEX(addr)] = ((uint32_t)data & PAGE_MASK) | FULL_ACCESS;
    }

    asm volatile("invlpg %0" : : "m"(addr) : "memory");
}

uint32_t process_schedule() {
    do {
        ctx.current_process = ctx.current_process->next;
    } while (ctx.current_process->state == PROC_ZOMBIE); // Skip zombies
    asm volatile("mov %0, %%cr3" :: "r"(ctx.current_process->page_directory) : "memory");
    return ctx.current_process->sp;
}
