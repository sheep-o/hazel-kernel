#include <../../../include/arch/i686/pic.h>
#include <../../../include/arch/i686/io.h>

/**
 * Sends an End of Interrupt (EOI) signal to the Programmable Interrupt Controller (PIC).
 *
 * @param irq The interrupt request line number for which the EOI signal is to be sent.
 *            If the irq number is greater than 7, the signal is sent to the secondary PIC.
 *            Otherwise, it is sent to the primary PIC.
 */
void pic_send_eoi(uint8_t irq) {
    if (irq > 7)
        outb(PIC_2_COMMAND, PIC_EOI);
    else
        outb(PIC_1_COMMAND, PIC_EOI);
}

/**
 * Remaps the Programmable Interrupt Controller (PIC) with new offsets for IRQ lines and unmasks the timer interrupt line.
 *
 * @param pic1_start The new offset for the master PIC's IRQ lines.
 * @param pic2_start The new offset for the slave PIC's IRQ lines.
 */
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

    // Unmask timer
    outb(PIC_1_DATA, 0b11111110);
    outb(PIC_2_DATA, 0b11111110);
}
