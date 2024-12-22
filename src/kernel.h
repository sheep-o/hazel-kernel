#ifndef HAZEL_KERNEL_H_
#define HAZEL_KERNEL_H_

#include <stdint.h>

struct mem_region {
    uint32_t start;
    uint32_t size;
};

#define MEM_REGIONS_MAX 3

struct k_ctx {
    uint64_t ticks;
    // For now allocate 3 structs for describing all free memory regions
    // Should be enough... for now...
    struct mem_region available_mem[MEM_REGIONS_MAX];
};

#endif
