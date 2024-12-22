ARCH = i686
CC = $(ARCH)-elf-gcc

BUILDDIR = build
SRC := $(wildcard src/*.S src/*.c src/arch/$(ARCH)/*.S src/arch/$(ARCH)/*.c)
OBJ := $(addprefix $(BUILDDIR)/, \
	   $(notdir \
	   $(patsubst %.S,%.o, $(patsubst %.c,%.o, $(SRC)))))
IMG := kernel.elf

CFLAGS    := -ffreestanding -Wall -Wextra -Werror -I src/arch/$(ARCH)
LDFLAGS   := -ffreestanding -nostdlib -lgcc -T src/arch/$(ARCH)/linker.ld
QEMUFLAGS := -kernel $(BUILDDIR)/$(IMG) -serial mon:stdio -s -m 1G

all: qemu

$(BUILDDIR)/$(IMG): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)
$(BUILDDIR)/%.o: src/%.S
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILDDIR)/%.o: src/arch/$(ARCH)/%.S
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/%.o: src/arch/$(ARCH)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

qemu: $(BUILDDIR)/$(IMG)
	qemu-system-i386 $(QEMUFLAGS)

clean:
	rm -rf build/*
