#include <multiboot.h>
#include <uart.h>
#include <util.h>

using namespace hzl;

#define CHECK_FLAG(x, n) ((x & (1 << n)))

extern "C" __attribute__((noreturn))
void bootstrap(const multiboot_info_t *info) {
  ASSERT(CHECK_FLAG(info->flags, 6), "Memory map must be provided by the bootloader");
  ASSERT(info->mods_count > 0, "Init module must be provided by bootloader");

  uart_init();
  uart_puts("Skibidi toilet\n");
  while (1);
}
