#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <elf.h>

#define STACK_SIZE 0x400000

enum PROCESS_STATE {
    PROC_OK,
    PROC_ZOMBIE,
};

struct p_ctx {
    uint32_t ebx;
    uint32_t ebp;
    uint32_t edi;
    uint32_t esi;
    uint32_t esp;
};

struct process {
    uint32_t sp;
    uint32_t ip;
    uint32_t page_directory;
    struct process *next;
    struct p_ctx c;
    uint32_t state;
    void *shm;
    uint32_t id;
};

struct int_ctx {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

void process_init(struct process *p, const Elf32_Ehdr *elf, bool change_page_dir);
void process_map(const struct process *p, const void *data, uint32_t addr);
uint32_t process_schedule();

#endif //PROCESS_H
