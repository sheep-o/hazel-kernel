#ifndef HAZEL_KERNEL_H_
#define HAZEL_KERNEL_H_

#include <stdint.h>
#include <elf.h>
#include <process.h>

#define KERNEL_VMA 0xC0000000
#define KERNEL_LMA 0x00100000
#define PAGE_SIZE 0x1000
#define PAGE_MASK 0xFFFFF000

struct k_ctx {
    struct process *current_process;
    uint64_t ticks;
    uint32_t mem_start;
    uint32_t mem_len;
    Elf32_Ehdr *init_elf;
};

#endif
