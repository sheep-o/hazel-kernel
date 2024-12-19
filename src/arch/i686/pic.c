#include "pic.h"
#include "io.h"

void pic_send_eoi(uint8_t irq) {
    if (irq > 7)
        outb(PIC_2_COMMAND, PIC_EOI);
    else
        outb(PIC_1_COMMAND, PIC_EOI);
}

void pic_remap(int pic1_start, int pic2_start) {
    // Mask all IRQ lines
    outb(PIC_1_DATA, 0xff);
    outb(PIC_2_DATA, 0xff);

    // Initialize master PIC
    outb(PIC_1_COMMAND, ICW4_NEEDED | INIT_BIT);
    outb(PIC_1_DATA, pic1_start);
    outb(PIC_1_DATA, PIC_1_ICW3);
    outb(PIC_1_DATA, 0x01);

    // Initialize slave PIC
    outb(PIC_2_COMMAND, ICW4_NEEDED | INIT_BIT);
    outb(PIC_2_DATA, pic2_start);
    outb(PIC_2_DATA, PIC_2_ICW3);
    outb(PIC_2_DATA, 0x01);

    // Unmask
    outb(PIC_1_DATA, 0b11111110);
    outb(PIC_2_DATA, 0b11111110);
}
