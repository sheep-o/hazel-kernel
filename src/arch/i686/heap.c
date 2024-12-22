#include "heap.h"
#include "../../kernel.h"
#include "../../printf.h"

extern struct k_ctx ctx;

void heap_init(void) {
    for (int i = 0; i < MEM_REGIONS_MAX; i++) {
        uint32_t start = ctx.available_mem[i].start;
        uint32_t size = ctx.available_mem[i].size;
        if (start + size) {
            printf("0x%08X-0x%08X\n", start, size);
        }
    }
}
