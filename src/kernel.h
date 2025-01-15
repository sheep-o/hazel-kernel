#ifndef HAZEL_KERNEL_H_
#define HAZEL_KERNEL_H_

#include <stdint.h>
#include <heap.h>
#include <elf.h>
#include <process.h>

struct mem_region {
    uint32_t start;
    uint32_t size;
};

#define MEM_REGIONS_MAX 3
#define KERNEL_VMA 0xC0000000
#define KERNEL_LMA 0x00100000
#define PAGE_SIZE 0x1000

struct k_ctx {
    uint64_t ticks;
    // For now allocate 3 structs for describing all free memory regions
    // Should be enough... for now...
    struct mem_region available_mem[MEM_REGIONS_MAX];
    struct node *free_mem_head;
    Elf32_Ehdr *init_elf;
    struct process *current_process;
};

#endif
