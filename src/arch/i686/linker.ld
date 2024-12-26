ENTRY(_start)

SECTIONS {
    . = 1M;
    .multiboot.data : {
        *(.multiboot.data)
    }
    .multiboot.text : {
        *(.multiboot.text)
    }

    /* Kernel will be mapped to the last GiB of memory */
    . = 0xC0101000;

    .text ALIGN(4K) : AT(ADDR (.text) - 0xC0000000) {
        *(.text)
    }
    .data ALIGN(4K) : AT(ADDR (.data) - 0xC0000000) {
        *(.data)
    }
    .rodata ALIGN(4K) : AT(ADDR (.rodata) - 0xC0000000) {
        *(.rodata)
    }
    .bss ALIGN(4K) : AT(ADDR (.bss) - 0xC0000000) {
        *(COMMON)
        *(.bss)

        . = ALIGN(16);
        . += 0x16000; /* 16 KiB of stack space */
        STACK_TOP = .;
    }
    KERNEL_END = .;
}
