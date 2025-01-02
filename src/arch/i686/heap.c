#include "heap.h"
#include "../../kernel.h"

extern struct k_ctx ctx;
extern uint32_t boot_page_dir[1024];
extern char KERNEL_END;

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

uint32_t tmp_page_tab[1024] __attribute__((aligned(0x1000)));

/**
 * Iterates through the available memory array and constructs a linked list of free memory regions.
 */
void heap_init(void) {
    for (int i = 0; i < MEM_REGIONS_MAX; i++) {
        uint32_t start = ctx.available_mem[i].start;
        uint32_t size = ctx.available_mem[i].size;
        if (size > 0) {
            // Temporarily map the start of the region to 0x0 for writing
            tmp_page_tab[0] = (start & 0xfffff000) | 7;
            boot_page_dir[0] = ((uint32_t)&tmp_page_tab - 0xC0000000) | 7;

            struct node *n = (struct node *)(0 + PAGE_OFFSET(start));
            n->size = size;
            if (i+1 < MEM_REGIONS_MAX) {
                n->next = (struct node *)ctx.available_mem[i+1].start;
            } else {
                n->next = 0;
            }

            boot_page_dir[0] = 2;
            asm volatile("invlpg (%0)" : : "r"(0) : "memory");
        }
    }

    ctx.free_mem_head = (struct node *)ctx.available_mem[0].start;
}

void idmap(uint32_t addr) {
    tmp_page_tab[PAGE_TAB_INDEX(addr)] = (addr & 0xfffff000) | 7;
    boot_page_dir[PAGE_DIR_INDEX(addr)] = ((uint32_t)&tmp_page_tab - 0xC0000000) | 7;
}
void un_idmap(uint32_t addr) {
    tmp_page_tab[PAGE_TAB_INDEX(addr)] = 2;
    boot_page_dir[PAGE_DIR_INDEX(addr)] = 2;
    asm volatile("invlpg (%0)" : : "r"(addr) : "memory");
}

/**
 * Traverses the free list for a memory region that can provide the requested number of bytes.
 * Note that the free region must be able to hold the length AND a res_hdr.
 * @param len The number of bytes to allocate
 * @return  A nonzero pointer to the allocated memory if successful
 */
void *heap_alloc(uint32_t len) {
    int i = 0;
    struct node *prev = 0;
    struct node *tmp = ctx.free_mem_head;
    // Using the i==0 conditional to let the loop run while the head is equal to 0
    while (tmp || i == 0) {
        i++;

        uint32_t req_size = len + sizeof(struct res_hdr);
        idmap((uint32_t)tmp);

        if (tmp->size > req_size) {
            uint32_t addr = (uint32_t)tmp + sizeof(struct res_hdr);
            tmp->size -= req_size;

            struct node *new_hdr = (struct node *)((uint32_t)tmp + req_size);
            idmap((uint32_t)new_hdr);
            new_hdr->size = tmp->size;
            new_hdr->next = tmp->next;

            if (i == 1) {
                ctx.free_mem_head = new_hdr;
            } else {
                idmap((uint32_t)prev);
                prev->next = (struct node *)((uint32_t)tmp + req_size);
                un_idmap((uint32_t)prev);
            }

            un_idmap((uint32_t)tmp);
            un_idmap((uint32_t)new_hdr);
            return (void *)addr;
        }

        if (tmp->size == len + sizeof(struct res_hdr)) {
            // TODO
            return 0; 
        }

        prev = tmp;
        tmp = tmp->next;
        un_idmap((uint32_t) prev);
    }

    return 0;
}

/**
 * Puts allocated memory back into the free list.
 * @param ptr A pointer to the allocated memory that needs to freed
 */
void heap_free(void *ptr) {
    struct res_hdr *hdr = (struct res_hdr *)((uint32_t)ptr - sizeof(struct res_hdr));
    (void)hdr;
}
