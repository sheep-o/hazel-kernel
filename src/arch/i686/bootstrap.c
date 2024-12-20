#include "multiboot.h"

#define CHECK_FLAG(x, n) ((x & (1 << n)))

extern void kmain(void);
void bootstrap(multiboot_info_t *info) {
    // Halt if memory map isn't provided
    if (!CHECK_FLAG(info->flags, 6)) return;

    kmain();
}
