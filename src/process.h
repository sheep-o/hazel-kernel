#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

struct process {
    uint32_t ip;
    uint32_t sp;
    uint32_t page_directory;
};


#define EI_NIDENT 16

typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;

typedef struct {
    uint8_t e_ident[EI_NIDENT];
    Elf32_Half      e_type;
    Elf32_Half      e_machine;
    Elf32_Word      e_version;
    Elf32_Addr      e_entry;
    Elf32_Off       e_phoff;
    Elf32_Off       e_shoff;
    Elf32_Word      e_flags;
    Elf32_Half      e_ehsize;
    Elf32_Half      e_phentsize;
    Elf32_Half      e_phnum;
    Elf32_Half      e_shentsize;
    Elf32_Half      e_shnum;
    Elf32_Half      e_shstrndx;
} elf_t;

typedef struct {
    Elf32_Word	sh_name;
    Elf32_Word	sh_type;
    Elf32_Word	sh_flags;
    Elf32_Addr	sh_addr;
    Elf32_Off	sh_offset;
    Elf32_Word	sh_size;
    Elf32_Word	sh_link;
    Elf32_Word	sh_info;
    Elf32_Word	sh_addralign;
    Elf32_Word	sh_entsize;
} section_t;

typedef struct {
    Elf32_Word	p_type;
    Elf32_Off	p_offset;
    Elf32_Addr	p_vaddr;
    Elf32_Addr	p_paddr;
    Elf32_Word	p_filesz;
    Elf32_Word	p_memsz;
    Elf32_Word	p_flags;
    Elf32_Word	p_align;
} phdr_t;

#define PT_NULL 	0
#define PT_LOAD 	1
#define PT_DYNAMIC 	2
#define PT_INTERP 	3
#define PT_NOTE 	4
#define PT_SHLIB 	5
#define PT_PHDR 	6
#define PT_TLS   	7
#define PT_LOOS 	0x60000000
#define PT_HIOS 	0x6fffffff
#define PT_LOPROC 	0x70000000
#define PT_HIPROC 	0x7fffffff

#endif //PROCESS_H
