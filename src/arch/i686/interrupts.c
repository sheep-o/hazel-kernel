#include <gdt.h>
#include <interrupts.h>
#include <pic.h>

struct idt_entry idt[IDT_MAX];

void int_encode_idt(int i, uint32_t offset, uint16_t segment_selector, uint8_t attributes) {
    idt[i].offset_lo = (uint16_t)(offset & 0xFFFF);
    idt[i].seg_select = segment_selector;
    idt[i].reserved = 0;
    idt[i].attributes = attributes;
    idt[i].offset_hi = (uint16_t)(offset >> 16);
}

void int_init() {
    for (int i = 0; i < EXCEPTION_COUNT; i++) {
        int_encode_idt(i, isr_stub_table[i], KERNEL_CODE_SEG,
            INT_PRESENT | TRAP_GATE_32 | INT_RING0);
    }

    int_encode_idt(PIT_INT_NUM, (uint32_t)pit_isr, KERNEL_CODE_SEG,
        INT_PRESENT | INT_GATE_32 | INT_RING0);
    int_encode_idt(SYS_INT_NUM, (uint32_t)syscall_isr, KERNEL_CODE_SEG,
        INT_PRESENT | INT_GATE_32 | INT_RING3);

    const struct idtr _idtr = {
        .base = (uint32_t)idt,
        .size = sizeof(struct idt_entry) * IDT_MAX - 1
    };

    asm volatile ("lidt %0" :: "m"(_idtr));
    pic_remap(PIC_1_START, PIC_2_START);
    EXIT_CRITICAL();
}