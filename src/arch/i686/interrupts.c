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
    for (int i = 0; i < 32; i++) {
        int_encode_idt(i, isr_stub_table[i], 0x8, INT_PRESENT | TRAP_GATE_32 | INT_RING0);
    }

    int_encode_idt(0x20, (uint32_t)pit_isr, 0x8, INT_PRESENT | INT_GATE_32 | INT_RING0);
    int_encode_idt(0x80, (uint32_t)syscall_isr, 0x8, INT_PRESENT | INT_GATE_32 | INT_RING3);

    const struct idtr _idtr = {
        .base = (uint32_t)idt,
        .size = (sizeof(struct idt_entry) * 256) - 1
    };

    asm volatile ("lidt %0" :: "m"(_idtr));
    pic_remap(PIC_1_START, PIC_2_START);
    asm volatile ("sti" ::);
}