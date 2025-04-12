const char hello[] = "A";

void _start(void) {
    while (1)
        asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));
}