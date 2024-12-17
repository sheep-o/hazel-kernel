ENTRY(_start)

SECTIONS {
    . = 1M;

    .text : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }
    .data : ALIGN(4K) {
        *(.data)
    }
    .rodata : ALIGN(4K) {
        *(.rodata)
    }
    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss)

        . = ALIGN(16);
        . += 0x16000; /* 16 KiB of stack space */
        STACK_TOP = .;
    }
}
