#include <allocator.h>
#include <kernel.h>
#include <util.h>
#include <interrupts.h>

extern struct k_ctx ctx;

static uint32_t bitmap_start = 0;
static uint32_t bitmap_size  = 0;
static uint32_t bitmap_data  = 0;

void alloc_init(void) {
    ENTER_CRITICAL();

    ASSERT(ctx.mem_start >= KERNEL_LMA,
           "Bitmap must start after the kernel to avoid low memory and more temporary page tables");
    ASSERT(ctx.mem_start < 0x400000,
           "Bitmap must start before 0x400000 to avoid temporary page tables");

    bitmap_size = ctx.mem_len / PAGE_SIZE;
    bitmap_size -= bitmap_size / PAGE_SIZE;

    ASSERT(ctx.mem_start + bitmap_size <= 0x400000,
           "Bitmap can't surpass 0x400000 to avoid temporary page tables");

    bitmap_start = KERNEL_VMA + ctx.mem_start;

    ctx.mem_start += bitmap_size;
    ctx.mem_len -= bitmap_size;

    bitmap_data = bitmap_start + bitmap_size/8;
    uint32_t rem = bitmap_data % PAGE_SIZE;
    if (rem != 0)
        bitmap_data += PAGE_SIZE - rem;

    for (uint32_t i = 0; i < bitmap_size; i++)
        alloc_free_page(i);

    EXIT_CRITICAL();
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
        if (!BIT(*byte, bit_num)) {
            alloc_res_page(i);
            return (void *) (bitmap_data + i * PAGE_SIZE);
        }
    }

    return 0;
}

bool alloc_is_free(uint32_t page_num) {
    uint8_t *byte = alloc_get_byte(page_num);
    const uint32_t bit_num = page_num % 8;
    return !BIT(*byte, bit_num);
}

void *alloc_consec(uint32_t num_pages) {
    uint32_t streak = 0;
    uint32_t i = 0;
    while (i < bitmap_size && streak < num_pages) {
        if (alloc_is_free(i))
            streak++;
        else
            streak = 0;

        i++;
    }

    if (streak == num_pages)
        return (void *) (bitmap_data + i * PAGE_SIZE);

    return 0;
}

void *alloc_enough(uint32_t size) {
    if (size % PAGE_SIZE != 0) size += PAGE_SIZE;
    return alloc_consec(size / PAGE_SIZE);
}