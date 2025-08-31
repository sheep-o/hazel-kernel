#include <multiboot.h>
#include <uart.h>
#include <kernel.h>
#include <util.h>

#define CHECK_FLAG(x, n) ((x & (1 << n)))
#define FLAG_MEM_MAP 6

#define MEMORY_PADDING 0x5000

extern struct k_ctx ctx;
extern void kmain(void);

void bootstrap(const multiboot_info_t *info) {
    ASSERT(CHECK_FLAG(info->flags, FLAG_MEM_MAP), "Memory map must be provided by the bootloader");
    ASSERT(info->mods_count > 0, "Init module must be provided by bootloader");

    const multiboot_module_t *mods = (multiboot_module_t *)(KERNEL_VMA + info->mods_addr);
    const multiboot_module_t *init = (multiboot_module_t *)(KERNEL_VMA + mods[0].mod_start);

    ctx.init_elf = (Elf32_Ehdr *)init;

    ASSERT(ctx.init_elf->e_ident[EI_MAG0] == ELFMAG0
        && ctx.init_elf->e_ident[EI_MAG1] == ELFMAG1
        && ctx.init_elf->e_ident[EI_MAG2] == ELFMAG2
        && ctx.init_elf->e_ident[EI_MAG3] == ELFMAG3,
        "Init module has an invalid ELF header");

    ASSERT(ctx.init_elf->e_ident[EI_CLASS] == ELFCLASS32, "Init module is not 32-bit");

    extern const char KERNEL_END;
    const uint32_t kernel_start = KERNEL_LMA;
    const uint32_t kernel_end = (uint32_t)&KERNEL_END - KERNEL_VMA;

    const uint32_t mmap_base = KERNEL_VMA + info->mmap_addr;
    const multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mmap_base;
    const multiboot_memory_map_t *max = NULL;
    const uint32_t mmap_entry_count = info->mmap_length / sizeof(multiboot_memory_map_t);

    for (uint32_t i = 0; i < mmap_entry_count; i++) {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t start = mmap[i].addr;
            uint32_t size = mmap[i].len;

            if (start >= kernel_start && start < kernel_end) {
                const uint32_t diff = kernel_end - start;
                size -= diff;
                start = kernel_end;
            }

            if (start < init->mod_end) {
                const uint32_t diff = init->mod_end - start;
                size -= diff;
                start = init->mod_end;
            }

            if (start == 0 && size > MEMORY_PADDING) {
                start += MEMORY_PADDING;
                size -= MEMORY_PADDING;
            }

            if (!max || max->size < size) {
              max = &mmap[i];
              ctx.mem_start = start;
              ctx.mem_len = size;
            }
        }
    }

    ASSERT(max, "Memory region not selected");
    ASSERT(ctx.mem_len > 0x100000*400, "Need at least 400 MiB of free memory");

    kmain();
}
