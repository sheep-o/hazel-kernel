#include <multiboot.h>
#include <uart.h>
#include <util.h>
#include <Kernel.h>

using namespace hzl;

#define CHECK_FLAG(x, n) ((x & (1 << n)))
#define KERNEL_VMA 0xC0000000

extern "C" __attribute__((noreturn))
void bootstrap(const multiboot_info_t *info) {
  ASSERT(CHECK_FLAG(info->flags, 6), "Memory map must be provided by the bootloader");
  ASSERT(info->mods_count > 0, "Init module must be provided by bootloader");

  const auto *mmap = (multiboot_memory_map_t *)(KERNEL_VMA + info->mmap_addr);
  Kernel kernel(mmap, info->mmap_length);
  kernel.Start();

  while (1);
}
