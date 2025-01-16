#include <uart.h>
#include <io.h>
#include <util.h>

uint8_t uart_init(void) {
    // Test COM1 by seeing if scratch register can store a value
    const uint8_t old_scratch = inb(COM1 + 7);
    outb(COM1 + 7, old_scratch + 1);
    ASSERT(inb(COM1 + 7) == old_scratch + 1, "COM1 scratch register failed to hold a value");
    outb(COM1 + 7, old_scratch);

    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    ASSERT(inb(COM1 + 0) == 0xAE, "COM1 loopback mode test failed");

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1 + 4, 0x0F);
    return 1;
}

void uart_putc(const char c) {
    // Poll until transmit is empty
    while(!IS_TRANSMIT_EMPTY(COM1)) asm volatile ("pause" ::);
    outb(COM1, c);
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s);
        s++;
    }
}
