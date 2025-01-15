const char hello[] = "Hello from user mode!\n";

void _start(void) {
    asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));
    while (1);
}