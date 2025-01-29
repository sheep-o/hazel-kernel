# Hazel Kernel

A simple microkernel I'm making to expand my OS knowledge (and for fun). Feel free to make pull requests or issues.

## Supported Architectures
- x86

## Build
#### Dependencies: cmake, ninja, grub2, xorriso, mtools
```
cmake . --toolchain src/arch/{ARCH}/{ARCH}.cmake
ninja iso
```

## Test
```
qemu-system-{ARCH} -cdrom Hazel.iso -m 1G -serial mon:stdio
```

## Todo
- Replace kernel heap with a page frame allocator?
- Add heap to usermode processes
- Free memory
- Interprocess communication
