#ifndef HAZEL_PIC_H_
#define HAZEL_PIC_H_

#define PIC_1_COMMAND 0x20
#define PIC_2_COMMAND 0xA0
#define PIC_1_DATA    0x21
#define PIC_2_DATA    0xA1

#define PIC_EOI 0x20

#define PIC_1_START 32
#define PIC_2_START  40

#define PIC_1_ICW3 0x04
#define PIC_2_ICW3 0x02 

#include <stdint.h>
#include <io.h>

enum ICW1 {
    ICW4_NEEDED = 0x01,
    SINGLE_MODE = 0x02,
    INTERVAL_4  = 0x04,
    LVL_TRIGGER = 0x08,
    INIT_BIT    = 0x10,
};

void pic_send_eoi(uint8_t irq);
void pic_remap(int pic1_start, int pic2_start);

#endif
