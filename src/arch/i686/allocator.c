#include <allocator.h>
#include <kernel.h>
#include <util.h>

extern struct k_ctx ctx;

static uint32_t bitmap_start = 0;
static uint32_t bitmap_size  = 0;

void alloc_init(void) {
    ASSERT(ctx.mem_start >= KERNEL_LMA,
           "Bitmap must start after the kernel to avoid low memory and more temporary page tables");
    ASSERT(ctx.mem_start < 0x400000,
           "Bitmap must start before 0x400000 to avoid temporary page tables");

    bitmap_size = ctx.mem_len / 0x1000;

    ASSERT(ctx.mem_start + bitmap_size <= 0x400000,
           "Bitmap can't surpass 0x400000 to avoid temporary page tables");

    bitmap_start = KERNEL_VMA + ctx.mem_start;

    ctx.mem_start += bitmap_size;
    ctx.mem_len -= bitmap_size;

    for (uint32_t i = 0; i < bitmap_size; i++)
        alloc_free_page(i);
}

uint8_t *alloc_get_byte(uint32_t page_num) {
    ASSERT(page_num < bitmap_size, "Trying to access an invalid page");
    const uint32_t byte_num = page_num / 8;
    const uint32_t addr = bitmap_start + byte_num;
    uint8_t *byte = (uint8_t *)addr;
    return byte;
}

void alloc_free_page(uint32_t page_num) {
    const uint32_t bit_num  = page_num % 8;
    const uint8_t mask = ~(1 << bit_num);
    uint8_t *byte = alloc_get_byte(page_num);
    ASSERT(byte, "Nullptr");
    *byte &= mask;
}

void alloc_res_page(uint32_t page_num) {
    uint8_t *byte = alloc_get_byte(page_num);
    const uint32_t bit_num  = page_num % 8;
    const uint8_t mask = 1 << bit_num;
    *byte |= mask;
}

#define BIT(x, n) ((x & (1 << n)) >> n)

void *alloc_first_page(void) {
    for (uint32_t i = 0; i < bitmap_size; i++) {
        uint8_t *byte = alloc_get_byte(i);
        const uint32_t bit_num = i % 8;
        if (!BIT(*byte, bit_num))
            return (void *)(i * 0x1000);
    }

    return 0;
}
