#include <user.h>
#include <heap.h>
#include <gdt.h>
#include <kernel.h>

struct tss_entry tss = {0};

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

extern struct k_ctx ctx;
extern uint32_t boot_page_dir[1024];
extern uint32_t boot_page_tab[1024];
extern uint32_t tmp_page_tab[1024];

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

void user_init(void) {
    uint32_t upage_dir = (uint32_t)heap_alloc(0x1000, true);
    uint32_t ucode_tab = (uint32_t)heap_alloc(0x1000, true);
    uint32_t ustack_tab = (uint32_t)heap_alloc(0x1000, true);
    uint32_t stack_bottom = (uint32_t)heap_alloc(0x400000, true);

    asm volatile("invlpg (%0)" : : "r"(0x1000) : "memory");
    asm volatile("invlpg (%0)" : : "r"(0x2000) : "memory");
    asm volatile("invlpg (%0)" : : "r"(0x3000) : "memory");
    // Map page directory to 0x1000
    boot_page_dir[PAGE_DIR_INDEX(0x1000)] = ((uint32_t)&tmp_page_tab - 0xC0000000) | 7;
    tmp_page_tab[PAGE_TAB_INDEX(0x1000)] = upage_dir | 7;
    // Map code table to 0x2000
    tmp_page_tab[PAGE_TAB_INDEX(0x2000)] = ucode_tab | 7;
    // Map stack table to 0x3000
    tmp_page_tab[PAGE_TAB_INDEX(0x3000)] = ustack_tab | 7;

    uint32_t *page_dir = (uint32_t *)0x1000;
    uint32_t *code_tab = (uint32_t *)0x2000;
    uint32_t *stack_tab = (uint32_t *)0x3000;

    // Map kernel
    page_dir[PAGE_DIR_INDEX(0xC0000000)] = ((uint32_t)boot_page_tab - 0xC0000000) | 7;

    // Map 4mib of stack space under the kernel
    page_dir[PAGE_DIR_INDEX(0xC0000000 - 1)] = ustack_tab | 7;
    for (int i = 0; i < 1024; i++)
        stack_tab[PAGE_TAB_INDEX(0xBFC00000 + i*0x1000)] = (stack_bottom + i*0x1000) | 7;

    // Map the load sections
    // TODO: Make this process not use two for loops
    phdr_t *phdr = (phdr_t *)((uint32_t)ctx.init_elf + ctx.init_elf->e_phoff);
    for (int i = 0; i < ctx.init_elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            uint8_t *s = (uint8_t *)heap_alloc(phdr[i].p_filesz, true);
            asm volatile("invlpg (%0)" : : "r"(0x2000) : "memory");

            // Map the ELF image to its specified entry
            page_dir[PAGE_DIR_INDEX(phdr[i].p_vaddr)] = ucode_tab | 7;
            code_tab[PAGE_TAB_INDEX(phdr[i].p_vaddr)] = (uint32_t)s | 7;
        }
    }
    asm volatile("mov %0, %%cr3" :: "r"(upage_dir) : "memory");
    for (int i = 0; i < ctx.init_elf->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            // Map the ELF image to its specified entry
            memcpy((void *)phdr[i].p_vaddr, (uint8_t *)ctx.init_elf + phdr[i].p_offset, phdr[i].p_filesz);
        }
    }

    // TODO: Improve readability... no 'magic' values
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = base + sizeof(struct tss_entry);
    gdt[5].base_low = base & 0xFFFF;
    gdt[5].base_middle = (base >> 16) & 0xFF;
    gdt[5].base_high = (base >> 24) & 0xFF;
    gdt[5].limit_low = limit & 0xFFFF;
    gdt[5].granularity = (limit >> 16) & 0x0F;
    gdt[5].granularity |= 0x40;
    gdt[5].access = 0x89;

    uint32_t esp0 = 0;
    asm volatile ("mov %%esp, %0" : "=r" (esp0));
    tss.esp0 = esp0;
    tss.ss0 = 0x10;
    tss.cs = 0x1B;
    tss.ds = 0x23;
    tss.es = 0x23;
    tss.fs = 0x23;
    tss.gs = 0x23;
    tss.ss = 0x23;
    asm volatile("ltr %%ax" : : "a"(5 << 3));

    struct process p = {
            .ip = ctx.init_elf->e_entry,
            .sp = 0xC0000000 - 0x4,
            .page_directory = upage_dir,
    };

    jmp_ring3(p.sp, p.ip);
}