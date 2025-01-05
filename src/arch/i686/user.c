#include <user.h>
#include <heap.h>
#include <gdt.h>

struct tss_entry tss = {0};

// TODO: dynamically allocate 4kb aligned page tables and directories for new processes
// TODO: Retrieve init program from external source such as grub module

// Temporary page table for init process
// Remove this after ability to dynamically allocate page tables is done
uint32_t cool_page_tab[1024] __attribute__((aligned(0x1000)));

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

void user(void) {
    while (1);
}

void user_init(void) {
    // TODO: Use a whole page table (4 mib) for the stack
    void *stack = heap_alloc(0x1000, false) + 0x1000;
    extern uint32_t boot_page_dir[1024];
    // Map the stack right under kernel space because the stack grows toward lower memory
    boot_page_dir[PAGE_DIR_INDEX(0xC0000000 - 1)] = ((uint32_t)&cool_page_tab - 0xC0000000) | 7;
    cool_page_tab[PAGE_TAB_INDEX(0xC0000000 - 1)] = ((uint32_t)stack & 0xFFFF000) | 7;

    boot_page_dir[PAGE_DIR_INDEX(0)] = ((uint32_t)&cool_page_tab - 0xC0000000) | 7;
    cool_page_tab[PAGE_TAB_INDEX(0)] = ((uint32_t)user & 0xFFFF000) | 7;

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

    jmp_ring3(0xC0000000 - 1, PAGE_OFFSET(user));
}