#include <vga.h>
#include <io.h>
#include <kernel.h>

/* Zero out VGA memory and set cursor to 0,0 */
void vga_clear(void) {
    uint16_t *vga_mem = (uint16_t *)(KERNEL_VMA + VGA_ADDR);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_mem[i] = 0;
    }
    vga_set_cursor_pos(0);
}

extern void *memcpy(void *dest, const void *src, int n);

void vga_putc(char c) {
    uint16_t pos = vga_get_cursor_pos();
    uint8_t *vmem = (uint8_t *)(KERNEL_VMA + VGA_ADDR);

    if (pos == VGA_WIDTH*VGA_HEIGHT) {
        if (c!= '\n') {
            // Move video memory up one row
            memcpy(vmem, vmem + VGA_WIDTH*2, VGA_WIDTH * (VGA_HEIGHT - 1) * 2);
            // Clear last row
            for (int i = 0; i < VGA_WIDTH*2; i+=2) {
                vmem[VGA_WIDTH * (VGA_HEIGHT - 1) * 2] = 0;
                vmem[VGA_WIDTH * (VGA_HEIGHT - 1) * 2 + 1] = VGA_COLOR_WHITE;
            }
            pos = VGA_WIDTH * (VGA_HEIGHT - 1);
        } else {
            // Move video memory up two rows
            memcpy(vmem, vmem + VGA_WIDTH*2*2, VGA_WIDTH * (VGA_HEIGHT - 2) * 2);
            // Clear last 2 rows
            for (int i = 0; i < VGA_WIDTH*2*2; i+=2) {
                ((char *)(vmem + VGA_WIDTH*(VGA_HEIGHT- 2)*2))[i] = 0;
                ((char *)(vmem + VGA_WIDTH*(VGA_HEIGHT- 2)*2))[i+1] = 0x0f;
            }
            pos = VGA_WIDTH * (VGA_HEIGHT - 2);
        }
    }

    char *video_memory = (char *)vmem + pos * 2;

    if (c == '\n') {
        uint16_t next_line = VGA_WIDTH * (pos / VGA_WIDTH + 1);
        vga_set_cursor_pos(next_line);
    } else {
        video_memory[0] = c;
        video_memory[1] = VGA_COLOR_WHITE;
        vga_set_cursor_pos(pos + 1);
    }
}



uint16_t vga_get_cursor_pos(void) {
    uint16_t pos = 0;

    /* Tell CRTC controller we want the low cursor position */
    outb(CRTC_ADDR_PORT, CRTC_CURSOR_LO_REGISTER);
    pos = inb(CRTC_DATA_PORT);

    /* Repeat with high position */
    outb(CRTC_ADDR_PORT, CRTC_CURSOR_HI_REGISTER);
    uint16_t hi_pos = 0;
    hi_pos = inb(CRTC_DATA_PORT);
    pos |= hi_pos << 8;

    return pos;
}

void vga_set_cursor_pos(uint16_t pos) {
    /* Tell CRTC controller we want the low cursor position */
    outb(CRTC_ADDR_PORT, CRTC_CURSOR_LO_REGISTER);
    outb(CRTC_DATA_PORT, (uint8_t)(pos & 0xFF));
    /* Repeat with high position */
    outb(CRTC_ADDR_PORT, CRTC_CURSOR_HI_REGISTER);
    outb(CRTC_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));
}
