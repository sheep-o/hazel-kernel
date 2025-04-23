const char hello[] = "Process 1\n";

void _start(void) {
    for (int i = 0; i < 1000; i++)
        asm volatile("int $0x80" : : "a"(1), "b"(1), "c"(hello), "d"(1));

    asm volatile("int $0x80" : : "a"(60));
}