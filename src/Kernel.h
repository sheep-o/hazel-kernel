//
// Created by rami on 4/12/25.
//

#ifndef HAZEL_KERNEL_KERNEL_H
#define HAZEL_KERNEL_KERNEL_H

#include <stdint.h>
#include <multiboot.h>

namespace hzl {

  class Kernel {
    uint32_t m_MemLen;
    uint32_t m_MemStart;
  public:
    Kernel(const multiboot_memory_map_t *mmap, multiboot_uint32_t len);
    void Start();
  };

} // hzl

#endif //HAZEL_KERNEL_KERNEL_H
