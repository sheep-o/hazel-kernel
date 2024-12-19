.section .text
.global _start
_start:
    cmpl $0x2BADB002, %eax
    jne halt

    lgdt (gdtp)
    movw $0x10, %ax
    movw %ax, %es
    movw %ax, %ds
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x8, $.use_code_seg
.use_code_seg:
    movl $STACK_TOP, %esp
    call kmain
    cli
halt:
    jmp halt

.global pit_isr
pit_isr:
    pusha
    call tmr_pit_handler
    popa
    iret

gdt:
gdt_null:
    .quad 0
gdt_code:
    .word 0xffff
    .word 0
    .byte 0
    .byte 0b10011010
    .byte 0b11001111
    .byte 0
gdt_data:
    .word 0xffff
    .word 0
    .byte 0
    .byte 0b10010010
    .byte 0b11001111
    .byte 0
gdt_end:

gdtp:
    .word gdt_end - gdt - 1
    .long gdt

.macro isr_err_stub i=0
    isr_stub_\i:
        hlt
        iret
.endm
.macro isr_no_err_stub i=0
    isr_stub_\i:
        hlt
        iret
.endm

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

.global isr_stub_table
isr_stub_table:
.long isr_stub_0
.long isr_stub_1
.long isr_stub_2
.long isr_stub_3
.long isr_stub_4
.long isr_stub_5
.long isr_stub_6
.long isr_stub_7
.long isr_stub_8
.long isr_stub_9
.long isr_stub_10
.long isr_stub_11
.long isr_stub_12
.long isr_stub_13
.long isr_stub_14
.long isr_stub_15
.long isr_stub_16
.long isr_stub_17
.long isr_stub_18
.long isr_stub_19
.long isr_stub_20
.long isr_stub_21
.long isr_stub_22
.long isr_stub_23
.long isr_stub_24
.long isr_stub_25
.long isr_stub_26
.long isr_stub_27
.long isr_stub_28
.long isr_stub_29
.long isr_stub_30
.long isr_stub_31
