#ifndef HAZEL_INTERRUPTS_H_
#define HAZEL_INTERRUPTS_H_

#include <stdint.h>

#define IDT_MAX 256
#define EXCEPTION_COUNT 32
#define PIT_INT_NUM 0x20
#define SYS_INT_NUM 0x80
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

/**
 * @brief A lookup table containing addresses to the ISRs for the 32 CPU exceptions, from start.S
 */
extern uint32_t isr_stub_table[EXCEPTION_COUNT];

/**
 * @brief Initialize the IDT. Handle CPU exceptions (0-31), the system timer interrupt (0x20), and the syscall interrupt (0x80)
 */
void int_init();

/**
 * @brief Encodes an entry in the Interrupt Descriptor Table (IDT)
 *
 * @param i The index of the IDT entry to encode
 * @param offset The 32-bit offset of the interrupt service routine
 * @param segment_selector The segment selector for the interrupt gate
 * @param attributes The attributes for the IDT entry, specifying gate type and privilege level
 */
void int_encode_idt(int i, uint32_t offset, uint16_t segment_selector, uint8_t attributes);

/**
 * @brief Initializes the Programmable Interrupt Controller (PIC)
 */
void int_pic_init();

/**
 * @brief Interrupt Service Routine (ISR) for the PIT, implemented in start.S
 */
extern void pit_isr();

/**
 * @brief Interrupt Service Routine (ISR) for syscalls, implemented in start.S
 */
extern void syscall_isr();


#endif
