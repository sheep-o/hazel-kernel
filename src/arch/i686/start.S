.section .multiboot.text, "ax", @progbits
.global _start
_start:
    cmpl $0x2BADB002, %eax
    jne halt

    // Create a page table for the first 4 MiB of physical memory
    mov $0, %eax
    mov $0, %edx
.fill_table:
    mov %edx, %ecx
    or $7, %ecx
    mov %ecx, boot_page_tab-0xC0000000(,%eax,4)
    inc %eax
    add $0x1000, %edx
    cmp $1024, %eax
    jne .fill_table

    mov $boot_page_tab, %eax
    sub $0xC0000000, %eax
    or $7, %eax
    // Identity map in order to execute the final jump instruction once paging is enabled
    xor %ecx, %ecx
    mov %eax, boot_page_dir-0xC0000000(,%ecx,4)
    // Map to 0xC0000000 (index 768 in page dir)
    mov $768, %ecx
    mov %eax, boot_page_dir-0xC0000000(,%ecx,4)

    // Enable paging
    mov $boot_page_dir, %eax
    sub $0xC0000000, %eax
    mov %eax, %cr3
    mov %cr0, %eax
    or $0x80000001, %eax
    mov %eax, %cr0
    jmp higher_half
    
.section .text
higher_half:
    // Remove identity mapping for kernel as it's no longer needed
    movl $2, (boot_page_dir)

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
    // Pointer to the multiboot info struct is stored in ebx
    add $0xC0000000, %ebx
    push %ebx
    call bootstrap
    cli
halt:
    jmp halt

.global jmp_ring3
jmp_ring3:
    mov 4(%esp), %ebx // stack pointer
    mov 8(%esp), %ecx // instruction pointer
    movw $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    // SS is handled by iret
    push $0x23
    push %ebx
    pushf
    push $0x1B
    push %ecx
    iret

.global pit_isr
pit_isr:
    call tmr_pit_handler
    test %eax, %eax
    jz .ret
    // Context switch time
    movl (ctx), %eax   // Get current process
    movl (%esp), %ebx
    movl %ebx, 4(%eax) // Save IP
    add $16, %eax      // Context
    movl %ebx, (%eax)
    movl %ebp, 4(%eax)
    movl %edi, 8(%eax)
    movl %esi, 12(%eax)
    movl %esp, 16(%eax)
    // This function will change the page directory and return the new stack pointer
    call process_schedule

    movl (ctx), %eax     // Get current process
    add $16, %eax        // Context
    mov 16(%eax), %esp   // Set SP
    movl (ctx), %eax     // Get current process
    mov 4(%eax), %ebx
    mov %ebx, (%esp)     // Restore SP

    add $16, %eax    // Context
    mov (%eax), %ebx
    mov 4(%eax), %ebp
    mov 8(%eax), %edi
    mov 12(%eax), %esi
    //mov %eax, %esp
    movw $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
.ret:
    iret

.global syscall_isr
syscall_isr:
    pusha
    call syscall_handler
    test %eax, %eax
    jz syscall.ret1
    // Context switch time
    movl (ctx), %eax // Get current process
    movl $1, 36(%eax)  // Set as zombie
    // This function will change the page directory and return the new stack pointer
    call process_schedule
    movl (ctx), %eax // Get current process
    add $16, %eax    // Context
    mov (%eax), %ebx
    mov 4(%eax), %ebp
    mov 8(%eax), %edi
    mov 12(%eax), %esi
    mov 16(%eax), %esp
    //mov %eax, %esp
    movw $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    jmp syscall.ret2
syscall.ret1:
    popa
syscall.ret2:
    iret

.macro isr_err_stub i=0
    isr_stub_\i:
        cli
        push $\i
        pusha
        call exception_handler
        popa
        add $8, %esp
        iret
.endm
.macro isr_no_err_stub i=0
    isr_stub_\i:
        cli
        push 0
        push $\i
        pusha
        call exception_handler
        popa
        add $12, %esp
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

.section .data
.global boot_page_tab
boot_page_tab:
.fill 1024, 4, 0

.global boot_page_dir
boot_page_dir:
.fill 1024, 4, 0

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

.global gdt
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
gdt_user_code:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b11111010
    .byte 0b11001111
    .byte 0x00
gdt_user_data:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b11110010
    .byte 0b11001111
    .byte 0x00
tss:
    .quad 0
gdt_end:

gdtp:
    .word gdt_end - gdt - 1
    .long gdt
