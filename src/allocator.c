#include <allocator.h>
#include <kernel.h>
#include <util.h>
#include <interrupts.h>

extern struct k_ctx ctx;

/* Starting address of bitmap */
static uint32_t bitmap_start = 0;
/* Number of pages the bitmap manages */
static uint32_t bitmap_len   = 0;
/* Size of the bitmap in pages */
static uint32_t bitmap_size  = 0;

void alloc_init(void) {
    ENTER_CRITICAL();

    ASSERT(ctx.mem_start >= KERNEL_LMA,
           "Bitmap must start after the kernel to avoid low memory and more temporary page tables");
    ASSERT(ctx.mem_start < 0x400000,
           "Bitmap must start before 0x400000 to avoid temporary page tables");

    // Align the start of the bitmap to a page boundary if not already aligned
    uint32_t rem = ctx.mem_start % PAGE_SIZE;
    if (rem != 0) {
        uint32_t offset = PAGE_SIZE - rem;
        ASSERT(ctx.mem_len >= offset, "Not enough memory to align the bitmap start");
        ctx.mem_start += offset;
        ctx.mem_len   -= offset;
    }

    bitmap_len = ctx.mem_len / PAGE_SIZE;
    bitmap_size = ((bitmap_len + 7) / 8 + PAGE_SIZE - 1) / PAGE_SIZE;

    ASSERT(ctx.mem_start + bitmap_size * PAGE_SIZE <= 0x400000,
           "Bitmap can't surpass 0x400000 to avoid temporary page tables");

    bitmap_start = KERNEL_VMA + ctx.mem_start;

    for (uint32_t i = 0; i < bitmap_len; i++)
        alloc_free_page(i);
    for (uint32_t i = 0; i < bitmap_size; i++)
        alloc_res_page(i);

    EXIT_CRITICAL();
}

uint8_t *alloc_get_byte(uint32_t page_num) {
    ASSERT(page_num < bitmap_len, "Trying to access an invalid page");
    const uint32_t byte_num = page_num / 8;
    const uint32_t addr = bitmap_start + byte_num;
    uint8_t *byte = (uint8_t *)addr;
    return byte;
}

void alloc_free_page(uint32_t page_num) {
    //ASSERT(page_num >= bitmap_size, "Cannot free a page that belongs to the bitmap");
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
    for (uint32_t i = 0; i < bitmap_len; i++) {
        uint8_t *byte = alloc_get_byte(i);
        const uint32_t bit_num = i % 8;
        if (!BIT(*byte, bit_num)) {
            alloc_res_page(i);
            return (void *) (bitmap_start + i * PAGE_SIZE);
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
    while (i < bitmap_len && streak < num_pages) {
        if (alloc_is_free(i))
            streak++;
        else
            streak = 0;

        i++;
    }

    if (streak == num_pages) {
        for (uint32_t j = 0; j < num_pages; j++)
            alloc_res_page(i - num_pages + j);

        return (void *) (bitmap_start + (i - num_pages) * PAGE_SIZE);
    }

    return 0;
}

void *alloc_enough(uint32_t size) {
    if (size == 0) return 0;

    uint32_t num_pages = size / PAGE_SIZE;
    if (size % PAGE_SIZE != 0) num_pages++;

    return alloc_consec(num_pages);
}