#ifndef HAZEL_INTERRUPTS_H_
#define HAZEL_INTERRUPTS_H_

#include <stdint.h>

#define IDT_MAX 256

#define ENTER_CRITICAL() asm volatile("cli" ::)
#define EXIT_CRITICAL() asm volatile("sti" ::)

struct idt_entry {
   uint16_t offset_lo; 
   uint16_t seg_select;
   uint8_t  reserved;
   uint8_t  attributes;
   uint16_t offset_hi;
} __attribute__ ((packed));

struct idtr {
    uint16_t size;
    uint32_t base;
} __attribute__ ((packed));

enum IDT_ATTRIBUTES {
    TASK_GATE    = 0x05,
    INT_GATE_16  = 0x06,
    TRAP_GATE_16 = 0x07,
    INT_GATE_32  = 0x0E,
    TRAP_GATE_32 = 0x0F,

    INT_RING0 = 0x00,
    INT_RING1 = 0x20,
    INT_RING2 = 0x40,
    INT_RING3 = 0x60,

    INT_PRESENT = 0x80,
};

void int_init();
void int_encode_idt(int i, uint32_t offset, uint16_t segment_selector, uint8_t attributes);
void int_pic_init();

#endif
