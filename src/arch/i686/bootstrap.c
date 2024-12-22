#include "multiboot.h"
#include "../../kernel.h"

#define CHECK_FLAG(x, n) ((x & (1 << n)))

extern struct k_ctx ctx;
extern void kmain(void);
void bootstrap(multiboot_info_t *info) {
    // Halt if memory map isn't provided
    if (!CHECK_FLAG(info->flags, 6)) return;

    int available_count = 0;
    uint32_t mmap_base = info->mmap_addr + 0xC0000000;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mmap_base;
    for (int i = 0; i*sizeof(multiboot_memory_map_t) < info->mmap_length && available_count < MEM_REGIONS_MAX; i++) {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            ctx.available_mem[available_count].size = (uint32_t)mmap[i].len;
            ctx.available_mem[available_count].start = (uint32_t)mmap[i].addr;
            available_count++;
        }
    }

    kmain();
}
