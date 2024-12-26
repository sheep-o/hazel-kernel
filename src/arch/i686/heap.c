#include "heap.h"
#include "../../kernel.h"

extern struct k_ctx ctx;
extern uint32_t boot_page_dir[1024];
extern char KERNEL_END;

#define PAGE_DIR_INDEX(x) ((((uint32_t)x) >> 22) & 0x3FF)
#define PAGE_TAB_INDEX(x) ((((uint32_t)x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x) (((uint32_t)x) & 0xFFF)

uint32_t tmp_page_tab[1024] __attribute__((aligned(0x1000)));

void heap_init(void) {
    for (int i = 0; i < MEM_REGIONS_MAX; i++) {
        uint32_t start = ctx.available_mem[i].start;
        uint32_t size = ctx.available_mem[i].size;
        if (start + size) {
            // Temporarily map the start of the region to 0x0
            tmp_page_tab[0] = (start & 0xfffff000) | 7;
            boot_page_dir[0] = ((uint32_t)&tmp_page_tab - 0xC0000000) | 7;

            struct node *n = (struct node *)PAGE_OFFSET(start);
            n->size = size;
            if (i+1 < MEM_REGIONS_MAX) {
                n->next = (struct node *)ctx.available_mem[i+1].start;
            } else {
                n->next = 0;
            }

            //Unmap it
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

void *heap_alloc(uint32_t len) {
    int i = 0;
    struct node *prev = 0;
    struct node *tmp = ctx.free_mem_head;
    while (tmp || i == 0) {
        i++;

        uint32_t start = (uint32_t)tmp;
        idmap(start);

        uint32_t addr = 0;

        if (tmp->size > len + sizeof(struct res_hdr)) {
            addr = start + sizeof(struct res_hdr);
            tmp->size -= len + sizeof(struct res_hdr);

            struct node *new_hdr = (struct node *)(start + sizeof(struct res_hdr) + len);
            idmap((uint32_t)new_hdr);
            new_hdr->size = tmp->size;
            new_hdr->next = tmp->next;

            if (i == 1) {
                ctx.free_mem_head = new_hdr;
            } else {
                idmap((uint32_t)prev);
                prev->next = (struct node *)((uint32_t)tmp + len + sizeof(struct res_hdr));
                un_idmap((uint32_t)prev);
            }

            un_idmap(start);
            un_idmap((uint32_t)new_hdr);
            return (void *)addr;
        } else if (tmp->size == len + sizeof(struct res_hdr)) {
            // TODO
            return 0; 
        }
        else {
            prev = tmp;
            tmp = tmp->next;
            un_idmap((uint32_t)prev);
            continue;
        }
    }

    return 0;
}

void free(uint32_t *addr) {
    //struct res_hdr *hdr = (struct res_hdr *)(addr - 2);
    (void)addr;
}
