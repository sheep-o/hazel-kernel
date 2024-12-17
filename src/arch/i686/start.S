.section .text
.global _start
_start:
    movb $'X', (0xb8000)
    movb $0x0f, (0xb8001)
    jmp .
