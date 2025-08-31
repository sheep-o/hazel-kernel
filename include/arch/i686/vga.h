#ifndef HAZEL_VGA_H_
#define HAZEL_VGA_H_

#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_ADDR   0xB8000

#define CRTC_ADDR_PORT 0x3d4
#define CRTC_DATA_PORT 0x3d5

#define CRTC_CURSOR_LO_REGISTER 0x0f
#define CRTC_CURSOR_HI_REGISTER 0x0e

enum VGA_COLOR {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
};

void vga_clear(void);
void vga_putc(char c);
uint16_t vga_get_cursor_pos(void);
void vga_set_cursor_pos(uint16_t pos);

#endif