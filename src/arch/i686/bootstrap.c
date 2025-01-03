#include "multiboot.h"
#include "../../kernel.h"

#define CHECK_FLAG(x, n) ((x & (1 << n)))

extern struct k_ctx ctx;
extern void kmain(void);
void bootstrap(multiboot_info_t *info) {
    // Halt if memory map isn't provided
    if (!CHECK_FLAG(info->flags, 6)) return;

    extern char KERNEL_END;
    uint32_t kernel_start = 0x100000;
    uint32_t kernel_end = (uint32_t)&KERNEL_END - 0xC0000000;

    int available_count = 0;
    uint32_t mmap_base = info->mmap_addr + 0xC0000000;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mmap_base;
    for (int i = 0; i*sizeof(multiboot_memory_map_t) < info->mmap_length && available_count < MEM_REGIONS_MAX; i++) {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t start = (uint32_t)mmap[i].addr;
            uint32_t size = (uint32_t)mmap[i].len;

            if (start >= kernel_start && start < kernel_end) {
                uint32_t diff = kernel_end - start;
                size -= diff;
                start = kernel_end;
            }

            ctx.available_mem[available_count].size = size;
            ctx.available_mem[available_count].start = start;
            available_count++;
        }
    }

    kmain();
}
