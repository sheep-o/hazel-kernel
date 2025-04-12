//
// Created by rami on 4/12/25.
//

#include <Kernel.h>
#include <uart.h>
#include <util.h>

namespace hzl {
  Kernel::Kernel(const multiboot_memory_map_t *mmap, multiboot_uint32_t len) {
    uart_init();

    const multiboot_memory_map_t *max_entry = nullptr;

    int count = len / sizeof(multiboot_memory_map_t);
    for (int i = 0; i < count; i++) {
      if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
        if (!max_entry || max_entry->len < mmap[i].len) {
          max_entry = &mmap[i];
        }
      }
    }

    ASSERT(max_entry, "Memory region not found\n");
    m_MemLen = max_entry->len;
    m_MemStart = max_entry->addr;
  }

  void Kernel::Start() {

  }
} // hzl