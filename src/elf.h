#ifndef HAZEL_ELF_H_
#define HAZEL_ELF_H_

typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;

/* Types for signed and unsigned 32-bit quantities.  */
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef int32_t  Elf64_Sword;

/* Types for signed and unsigned 64-bit quantities.  */
typedef uint64_t Elf32_Xword;
typedef int64_t  Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

/* Type of addresses.  */
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;

/* Type of file offsets.  */
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;

/* Type for section indices, which are 16-bit quantities.  */
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;

/* Type for version symbol information.  */
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

/* Type for number of section header table entries */
typedef Elf32_Word Elf32_Shnum;
typedef Elf64_Xword Elf64_Shnum;

/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT (16)

typedef struct
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
  Elf32_Half    e_type;         /* Object file type */
  Elf32_Half    e_machine;      /* Architecture */
  Elf32_Word    e_version;      /* Object file version */
  Elf32_Addr    e_entry;        /* Entry point virtual address */
  Elf32_Off e_phoff;        /* Program header table file offset */
  Elf32_Off e_shoff;        /* Section header table file offset */
  Elf32_Word    e_flags;        /* Processor-specific flags */
  Elf32_Half    e_ehsize;       /* ELF header size in bytes */
  Elf32_Half    e_phentsize;        /* Program header table entry size */
  Elf32_Half    e_phnum;        /* Program header table entry count */
  Elf32_Half    e_shentsize;        /* Section header table entry size */
  Elf32_Half    e_shnum;        /* Section header table entry count */
  Elf32_Half    e_shstrndx;     /* Section header string table index */
} Elf32_Ehdr;

typedef struct
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
  Elf64_Half    e_type;         /* Object file type */
  Elf64_Half    e_machine;      /* Architecture */
  Elf64_Word    e_version;      /* Object file version */
  Elf64_Addr    e_entry;        /* Entry point virtual address */
  Elf64_Off e_phoff;        /* Program header table file offset */
  Elf64_Off e_shoff;        /* Section header table file offset */
  Elf64_Word    e_flags;        /* Processor-specific flags */
  Elf64_Half    e_ehsize;       /* ELF header size in bytes */
  Elf64_Half    e_phentsize;        /* Program header table entry size */
  Elf64_Half    e_phnum;        /* Program header table entry count */
  Elf64_Half    e_shentsize;        /* Section header table entry size */
  Elf64_Half    e_shnum;        /* Section header table entry count */
  Elf64_Half    e_shstrndx;     /* Section header string table index */
} Elf64_Ehdr;

/* Fields in the e_ident array.  The EI_* macros are indices into the
   array.  The macros under each EI_* macro are the values the byte
   may have.  */

#define EI_MAG0     0       /* File identification byte 0 index */
#define ELFMAG0     0x7f        /* Magic number byte 0 */

#define EI_MAG1     1       /* File identification byte 1 index */
#define ELFMAG1     'E'     /* Magic number byte 1 */

#define EI_MAG2     2       /* File identification byte 2 index */
#define ELFMAG2     'L'     /* Magic number byte 2 */

#define EI_MAG3     3       /* File identification byte 3 index */
#define ELFMAG3     'F'     /* Magic number byte 3 */

/* Conglomeration of the identification bytes, for easy testing as a word.  */
#define ELFMAG      "\177ELF"
#define SELFMAG     4

#define EI_CLASS    4       /* File class byte index */
#define ELFCLASSNONE    0       /* Invalid class */
#define ELFCLASS32  1       /* 32-bit objects */
#define ELFCLASS64  2       /* 64-bit objects */
#define ELFCLASSNUM 3

#define EI_DATA     5       /* Data encoding byte index */
#define ELFDATANONE 0       /* Invalid data encoding */
#define ELFDATA2LSB 1       /* 2's complement, little endian */
#define ELFDATA2MSB 2       /* 2's complement, big endian */
#define ELFDATANUM  3

#define EI_VERSION  6       /* File version byte index */
                    /* Value must be EV_CURRENT */

#define EI_OSABI    7       /* OS ABI identification */
#define ELFOSABI_NONE       0   /* UNIX System V ABI */
#define ELFOSABI_SYSV       0   /* Alias.  */
#define ELFOSABI_HPUX       1   /* HP-UX */
#define ELFOSABI_NETBSD     2   /* NetBSD.  */
#define ELFOSABI_LINUX      3   /* Linux.  */
#define ELFOSABI_SOLARIS    6   /* Sun Solaris.  */
#define ELFOSABI_AIX        7   /* IBM AIX.  */
#define ELFOSABI_IRIX       8   /* SGI Irix.  */
#define ELFOSABI_FREEBSD    9   /* FreeBSD.  */
#define ELFOSABI_TRU64      10  /* Compaq TRU64 UNIX.  */
#define ELFOSABI_MODESTO    11  /* Novell Modesto.  */
#define ELFOSABI_OPENBSD    12  /* OpenBSD.  */
#define ELFOSABI_ARM_AEABI  64  /* ARM EABI */
#define ELFOSABI_ARM        97  /* ARM */
#define ELFOSABI_STANDALONE 255 /* Standalone (embedded) application */

#define EI_ABIVERSION   8       /* ABI version */

#define EI_PAD      9       /* Byte index of padding bytes */

/* Legal values for e_type (object file type).  */

#define ET_NONE     0       /* No file type */
#define ET_REL      1       /* Relocatable file */
#define ET_EXEC     2       /* Executable file */
#define ET_DYN      3       /* Shared object file */
#define ET_CORE     4       /* Core file */
#define ET_NUM      5       /* Number of defined types */
#define ET_LOOS     0xfe00      /* OS-specific range start */
#define ET_HIOS     0xfeff      /* OS-specific range end */
#define ET_LOPROC   0xff00      /* Processor-specific range start */
#define ET_HIPROC   0xffff      /* Processor-specific range end */

/* Legal values for e_machine (architecture).  */

#define EM_NONE      0      /* No machine */
#define EM_M32       1      /* AT&T WE 32100 */
#define EM_SPARC     2      /* SUN SPARC */
#define EM_386       3      /* Intel 80386 */
#define EM_68K       4      /* Motorola m68k family */
#define EM_88K       5      /* Motorola m88k family */
#define EM_860       7      /* Intel 80860 */
#define EM_MIPS      8      /* MIPS R3000 big-endian */
#define EM_S370      9      /* IBM System/370 */
#define EM_MIPS_RS3_LE  10      /* MIPS R3000 little-endian */

#define EM_PARISC   15      /* HPPA */
#define EM_VPP500   17      /* Fujitsu VPP500 */
#define EM_SPARC32PLUS  18      /* Sun's "v8plus" */
#define EM_960      19      /* Intel 80960 */
#define EM_PPC      20      /* PowerPC */
#define EM_PPC64    21      /* PowerPC 64-bit */
#define EM_S390     22      /* IBM S390 */

#define EM_V800     36      /* NEC V800 series */
#define EM_FR20     37      /* Fujitsu FR20 */
#define EM_RH32     38      /* TRW RH-32 */
#define EM_RCE      39      /* Motorola RCE */
#define EM_ARM      40      /* ARM */
#define EM_FAKE_ALPHA   41      /* Digital Alpha */
#define EM_SH       42      /* Hitachi SH */
#define EM_SPARCV9  43      /* SPARC v9 64-bit */
#define EM_TRICORE  44      /* Siemens Tricore */
#define EM_ARC      45      /* Argonaut RISC Core */
#define EM_H8_300   46      /* Hitachi H8/300 */
#define EM_H8_300H  47      /* Hitachi H8/300H */
#define EM_H8S      48      /* Hitachi H8S */
#define EM_H8_500   49      /* Hitachi H8/500 */
#define EM_IA_64    50      /* Intel Merced */
#define EM_MIPS_X   51      /* Stanford MIPS-X */
#define EM_COLDFIRE 52      /* Motorola Coldfire */
#define EM_68HC12   53      /* Motorola M68HC12 */
#define EM_MMA      54      /* Fujitsu MMA Multimedia Accelerator*/
#define EM_PCP      55      /* Siemens PCP */
#define EM_NCPU     56      /* Sony nCPU embedded RISC */
#define EM_NDR1     57      /* Denso NDR1 microprocessor */
#define EM_STARCORE 58      /* Motorola Start*Core processor */
#define EM_ME16     59      /* Toyota ME16 processor */
#define EM_ST100    60      /* STMicroelectronics ST100 processor */
#define EM_TINYJ    61      /* Advanced Logic Corp. Tinyj emb.fam*/
#define EM_X86_64   62      /* AMD x86-64 architecture */
#define EM_PDSP     63      /* Sony DSP Processor */

#define EM_FX66     66      /* Siemens FX66 microcontroller */
#define EM_ST9PLUS  67      /* STMicroelectronics ST9+ 8/16 mc */
#define EM_ST7      68      /* STMicroelectronics ST7 8 bit mc */
#define EM_68HC16   69      /* Motorola MC68HC16 microcontroller */
#define EM_68HC11   70      /* Motorola MC68HC11 microcontroller */
#define EM_68HC08   71      /* Motorola MC68HC08 microcontroller */
#define EM_68HC05   72      /* Motorola MC68HC05 microcontroller */
#define EM_SVX      73      /* Silicon Graphics SVx */
#define EM_AT19     74      /* STMicroelectronics ST19 8 bit mc */
#define EM_VAX      75      /* Digital VAX */
#define EM_CRIS     76      /* Axis Communications 32-bit embedded processor */
#define EM_JAVELIN  77      /* Infineon Technologies 32-bit embedded processor */
#define EM_FIREPATH 78      /* Element 14 64-bit DSP Processor */
#define EM_ZSP      79      /* LSI Logic 16-bit DSP Processor */
#define EM_MMIX     80      /* Donald Knuth's educational 64-bit processor */
#define EM_HUANY    81      /* Harvard University machine-independent object files */
#define EM_PRISM    82      /* SiTera Prism */
#define EM_AVR      83      /* Atmel AVR 8-bit microcontroller */
#define EM_FR30     84      /* Fujitsu FR30 */
#define EM_D10V     85      /* Mitsubishi D10V */
#define EM_D30V     86      /* Mitsubishi D30V */
#define EM_V850     87      /* NEC v850 */
#define EM_M32R     88      /* Mitsubishi M32R */
#define EM_MN10300  89      /* Matsushita MN10300 */
#define EM_MN10200  90      /* Matsushita MN10200 */
#define EM_PJ       91      /* picoJava */
#define EM_OPENRISC 92      /* OpenRISC 32-bit embedded processor */
#define EM_ARC_A5   93      /* ARC Cores Tangent-A5 */
#define EM_XTENSA   94      /* Tensilica Xtensa Architecture */
#define EM_NUM      95
#define EM_AARCH64  183     /* ARM 64-bit architecture */
#define EM_RISCV    243     /* RISC-V */
#define EM_LOONGARCH    258     /* LoongArch */

/* If it is necessary to assign new unofficial EM_* values, please
   pick large random numbers (0x8523, 0xa7f2, etc.) to minimize the
   chances of collision with official or non-GNU unofficial values.  */

#define EM_ALPHA    0x9026

/* Legal values for e_version (version).  */

#define EV_NONE     0       /* Invalid ELF version */
#define EV_CURRENT  1       /* Current version */
#define EV_NUM      2

/* Section header.  */

typedef struct
{
  Elf32_Word    sh_name;        /* Section name (string tbl index) */
  Elf32_Word    sh_type;        /* Section type */
  Elf32_Word    sh_flags;       /* Section flags */
  Elf32_Addr    sh_addr;        /* Section virtual addr at execution */
  Elf32_Off sh_offset;      /* Section file offset */
  Elf32_Word    sh_size;        /* Section size in bytes */
  Elf32_Word    sh_link;        /* Link to another section */
  Elf32_Word    sh_info;        /* Additional section information */
  Elf32_Word    sh_addralign;       /* Section alignment */
  Elf32_Word    sh_entsize;     /* Entry size if section holds table */
} Elf32_Shdr;

typedef struct
{
  Elf64_Word    sh_name;        /* Section name (string tbl index) */
  Elf64_Word    sh_type;        /* Section type */
  Elf64_Xword   sh_flags;       /* Section flags */
  Elf64_Addr    sh_addr;        /* Section virtual addr at execution */
  Elf64_Off sh_offset;      /* Section file offset */
  Elf64_Xword   sh_size;        /* Section size in bytes */
  Elf64_Word    sh_link;        /* Link to another section */
  Elf64_Word    sh_info;        /* Additional section information */
  Elf64_Xword   sh_addralign;       /* Section alignment */
  Elf64_Xword   sh_entsize;     /* Entry size if section holds table */
} Elf64_Shdr;

/* Special section indices.  */

#define SHN_UNDEF   0       /* Undefined section */
#define SHN_LORESERVE   0xff00      /* Start of reserved indices */
#define SHN_LOPROC  0xff00      /* Start of processor-specific */
#define SHN_HIPROC  0xff1f      /* End of processor-specific */
#define SHN_LOOS    0xff20      /* Start of OS-specific */
#define SHN_HIOS    0xff3f      /* End of OS-specific */
#define SHN_ABS     0xfff1      /* Associated symbol is absolute */
#define SHN_COMMON  0xfff2      /* Associated symbol is common */
#define SHN_XINDEX  0xffff      /* Index is in extra table.  */
#define SHN_HIRESERVE   0xffff      /* End of reserved indices */

/* Legal values for sh_type (section type).  */

#define SHT_NULL      0     /* Section header table entry unused */
#define SHT_PROGBITS      1     /* Program data */
#define SHT_SYMTAB    2     /* Symbol table */
#define SHT_STRTAB    3     /* String table */
#define SHT_RELA      4     /* Relocation entries with addends */
#define SHT_HASH      5     /* Symbol hash table */
#define SHT_DYNAMIC   6     /* Dynamic linking information */
#define SHT_NOTE      7     /* Notes */
#define SHT_NOBITS    8     /* Program space with no data (bss) */
#define SHT_REL       9     /* Relocation entries, no addends */
#define SHT_SHLIB     10        /* Reserved */
#define SHT_DYNSYM    11        /* Dynamic linker symbol table */
#define SHT_INIT_ARRAY    14        /* Array of constructors */
#define SHT_FINI_ARRAY    15        /* Array of destructors */
#define SHT_PREINIT_ARRAY 16        /* Array of pre-constructors */
#define SHT_GROUP     17        /* Section group */
#define SHT_SYMTAB_SHNDX  18        /* Extended section indices */
#define SHT_NUM       19        /* Number of defined types.  */
#define SHT_LOOS      0x60000000    /* Start OS-specific */
#define SHT_GNU_LIBLIST   0x6ffffff7    /* Prelink library list */
#define SHT_CHECKSUM      0x6ffffff8    /* Checksum for DSO content.  */
#define SHT_LOSUNW    0x6ffffffa    /* Sun-specific low bound.  */
#define SHT_SUNW_move     0x6ffffffa
#define SHT_SUNW_COMDAT   0x6ffffffb
#define SHT_SUNW_syminfo  0x6ffffffc
#define SHT_GNU_verdef    0x6ffffffd    /* Version definition section.  */
#define SHT_GNU_verneed   0x6ffffffe    /* Version needs section.  */
#define SHT_GNU_versym    0x6fffffff    /* Version symbol table.  */
#define SHT_HISUNW    0x6fffffff    /* Sun-specific high bound.  */
#define SHT_HIOS      0x6fffffff    /* End OS-specific type */
#define SHT_LOPROC    0x70000000    /* Start of processor-specific */
#define SHT_HIPROC    0x7fffffff    /* End of processor-specific */
#define SHT_LOUSER    0x80000000    /* Start of application-specific */
#define SHT_HIUSER    0x8fffffff    /* End of application-specific */

/* Legal values for sh_flags (section flags).  */

#define SHF_WRITE        (1 << 0)   /* Writable */
#define SHF_ALLOC        (1 << 1)   /* Occupies memory during execution */
#define SHF_EXECINSTR        (1 << 2)   /* Executable */
#define SHF_MERGE        (1 << 4)   /* Might be merged */
#define SHF_STRINGS      (1 << 5)   /* Contains nul-terminated strings */
#define SHF_INFO_LINK        (1 << 6)   /* `sh_info' contains SHT index */
#define SHF_LINK_ORDER       (1 << 7)   /* Preserve order after combining */
#define SHF_OS_NONCONFORMING (1 << 8)   /* Non-standard OS specific handling
                       required */
#define SHF_GROUP        (1 << 9)   /* Section is member of a group.  */
#define SHF_TLS          (1 << 10)  /* Section hold thread-local data.  */
#define SHF_MASKOS       0x0ff00000 /* OS-specific.  */
#define SHF_MASKPROC         0xf0000000 /* Processor-specific */

/* Section group handling.  */
#define GRP_COMDAT  0x1     /* Mark group as COMDAT.  */

/* Symbol table entry.  */

typedef struct
{
  Elf32_Word    st_name;        /* Symbol name (string tbl index) */
  Elf32_Addr    st_value;       /* Symbol value */
  Elf32_Word    st_size;        /* Symbol size */
  unsigned char st_info;        /* Symbol type and binding */
  unsigned char st_other;       /* Symbol visibility */
  Elf32_Section st_shndx;       /* Section index */
} Elf32_Sym;

typedef struct
{
  Elf64_Word    st_name;        /* Symbol name (string tbl index) */
  unsigned char st_info;        /* Symbol type and binding */
  unsigned char st_other;       /* Symbol visibility */
  Elf64_Section st_shndx;       /* Section index */
  Elf64_Addr    st_value;       /* Symbol value */
  Elf64_Xword   st_size;        /* Symbol size */
} Elf64_Sym;

/* The syminfo section if available contains additional information about
   every dynamic symbol.  */

typedef struct
{
  Elf32_Half si_boundto;        /* Direct bindings, symbol bound to */
  Elf32_Half si_flags;          /* Per symbol flags */
} Elf32_Syminfo;

typedef struct
{
  Elf64_Half si_boundto;        /* Direct bindings, symbol bound to */
  Elf64_Half si_flags;          /* Per symbol flags */
} Elf64_Syminfo;

/* Possible values for si_boundto.  */
#define SYMINFO_BT_SELF     0xffff  /* Symbol bound to self */
#define SYMINFO_BT_PARENT   0xfffe  /* Symbol bound to parent */
#define SYMINFO_BT_LOWRESERVE   0xff00  /* Beginning of reserved entries */

/* Possible bitmasks for si_flags.  */
#define SYMINFO_FLG_DIRECT  0x0001  /* Direct bound symbol */
#define SYMINFO_FLG_PASSTHRU    0x0002  /* Pass-thru symbol for translator */
#define SYMINFO_FLG_COPY    0x0004  /* Symbol is a copy-reloc */
#define SYMINFO_FLG_LAZYLOAD    0x0008  /* Symbol bound to object to be lazy
                       loaded */
/* Syminfo version values.  */
#define SYMINFO_NONE        0
#define SYMINFO_CURRENT     1
#define SYMINFO_NUM     2


/* How to extract and insert information held in the st_info field.  */

#define ELF32_ST_BIND(val)      (((unsigned char) (val)) >> 4)
#define ELF32_ST_TYPE(val)      ((val) & 0xf)
#define ELF32_ST_INFO(bind, type)   (((bind) << 4) + ((type) & 0xf))

/* Both Elf32_Sym and Elf64_Sym use the same one-byte st_info field.  */
#define ELF64_ST_BIND(val)      ELF32_ST_BIND (val)
#define ELF64_ST_TYPE(val)      ELF32_ST_TYPE (val)
#define ELF64_ST_INFO(bind, type)   ELF32_ST_INFO ((bind), (type))

/* Legal values for ST_BIND subfield of st_info (symbol binding).  */

#define STB_LOCAL   0       /* Local symbol */
#define STB_GLOBAL  1       /* Global symbol */
#define STB_WEAK    2       /* Weak symbol */
#define STB_NUM     3       /* Number of defined types.  */
#define STB_LOOS    10      /* Start of OS-specific */
#define STB_HIOS    12      /* End of OS-specific */
#define STB_LOPROC  13      /* Start of processor-specific */
#define STB_HIPROC  15      /* End of processor-specific */

/* Legal values for ST_TYPE subfield of st_info (symbol type).  */

#define STT_NOTYPE  0       /* Symbol type is unspecified */
#define STT_OBJECT  1       /* Symbol is a data object */
#define STT_FUNC    2       /* Symbol is a code object */
#define STT_SECTION 3       /* Symbol associated with a section */
#define STT_FILE    4       /* Symbol's name is file name */
#define STT_COMMON  5       /* Symbol is a common data object */
#define STT_TLS     6       /* Symbol is thread-local data object*/
#define STT_NUM     7       /* Number of defined types.  */
#define STT_LOOS    10      /* Start of OS-specific */
#define STT_HIOS    12      /* End of OS-specific */
#define STT_LOPROC  13      /* Start of processor-specific */
#define STT_HIPROC  15      /* End of processor-specific */


/* Symbol table indices are found in the hash buckets and chain table
   of a symbol hash table section.  This special index value indicates
   the end of a chain, meaning no further symbols are found in that bucket.  */

#define STN_UNDEF   0       /* End of a chain.  */
#define STN_ABS     65521


/* How to extract and insert information held in the st_other field.  */

#define ELF32_ST_VISIBILITY(o)  ((o) & 0x03)

/* For ELF64 the definitions are the same.  */
#define ELF64_ST_VISIBILITY(o)  ELF32_ST_VISIBILITY (o)

/* Symbol visibility specification encoded in the st_other field.  */
#define STV_DEFAULT 0       /* Default symbol visibility rules */
#define STV_INTERNAL    1       /* Processor specific hidden class */
#define STV_HIDDEN  2       /* Sym unavailable in other modules */
#define STV_PROTECTED   3       /* Not preemptible, not exported */


/* Relocation table entry without addend (in section of type SHT_REL).  */

typedef struct
{
  Elf32_Addr    r_offset;       /* Address */
  Elf32_Word    r_info;         /* Relocation type and symbol index */
} Elf32_Rel;

/* I have seen two different definitions of the Elf64_Rel and
   Elf64_Rela structures, so we'll leave them out until Novell (or
   whoever) gets their act together.  */
/* The following, at least, is used on Sparc v9, MIPS, and Alpha.  */

typedef struct
{
  Elf64_Addr    r_offset;       /* Address */
  Elf64_Xword   r_info;         /* Relocation type and symbol index */
} Elf64_Rel;

/* Relocation table entry with addend (in section of type SHT_RELA).  */

typedef struct
{
  Elf32_Addr    r_offset;       /* Address */
  Elf32_Word    r_info;         /* Relocation type and symbol index */
  Elf32_Sword   r_addend;       /* Addend */
} Elf32_Rela;

typedef struct
{
  Elf64_Addr    r_offset;       /* Address */
  Elf64_Xword   r_info;         /* Relocation type and symbol index */
  Elf64_Sxword  r_addend;       /* Addend */
} Elf64_Rela;

/* How to extract and insert information held in the r_info field.  */

#define ELF32_R_SYM(val)        ((val) >> 8)
#define ELF32_R_TYPE(val)       ((val) & 0xff)
#define ELF32_R_INFO(sym, type)     (((sym) << 8) + ((type) & 0xff))

#define ELF64_R_SYM(i)          ((i) >> 32)
#define ELF64_R_TYPE(i)         ((i) & 0xffffffff)
#define ELF64_R_INFO(sym,type)      ((((Elf64_Xword) (sym)) << 32) + (type))

/* Program segment header.  */

typedef struct
{
  Elf32_Word    p_type;         /* Segment type */
  Elf32_Off p_offset;       /* Segment file offset */
  Elf32_Addr    p_vaddr;        /* Segment virtual address */
  Elf32_Addr    p_paddr;        /* Segment physical address */
  Elf32_Word    p_filesz;       /* Segment size in file */
  Elf32_Word    p_memsz;        /* Segment size in memory */
  Elf32_Word    p_flags;        /* Segment flags */
  Elf32_Word    p_align;        /* Segment alignment */
} Elf32_Phdr;

typedef struct
{
  Elf64_Word    p_type;         /* Segment type */
  Elf64_Word    p_flags;        /* Segment flags */
  Elf64_Off p_offset;       /* Segment file offset */
  Elf64_Addr    p_vaddr;        /* Segment virtual address */
  Elf64_Addr    p_paddr;        /* Segment physical address */
  Elf64_Xword   p_filesz;       /* Segment size in file */
  Elf64_Xword   p_memsz;        /* Segment size in memory */
  Elf64_Xword   p_align;        /* Segment alignment */
} Elf64_Phdr;

/* Legal values for p_type (segment type).  */

#define PT_NULL     0       /* Program header table entry unused */
#define PT_LOAD     1       /* Loadable program segment */
#define PT_DYNAMIC  2       /* Dynamic linking information */
#define PT_INTERP   3       /* Program interpreter */
#define PT_NOTE     4       /* Auxiliary information */
#define PT_SHLIB    5       /* Reserved */
#define PT_PHDR     6       /* Entry for header table itself */
#define PT_TLS      7       /* Thread-local storage segment */
#define PT_NUM      8       /* Number of defined types */
#define PT_LOOS     0x60000000  /* Start of OS-specific */
#define PT_GNU_EH_FRAME 0x6474e550  /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK    0x6474e551      /* GCC stack segment  */
#define PT_LOSUNW   0x6ffffffa
#define PT_SUNWBSS  0x6ffffffa  /* Sun Specific segment */
#define PT_SUNWSTACK    0x6ffffffb  /* Stack segment */
#define PT_HISUNW   0x6fffffff
#define PT_HIOS     0x6fffffff  /* End of OS-specific */
#define PT_LOPROC   0x70000000  /* Start of processor-specific */
#define PT_HIPROC   0x7fffffff  /* End of processor-specific */
#define PN_XNUM     0xffff

/* Legal values for p_flags (segment flags).  */

#define PF_X        (1 << 0)    /* Segment is executable */
#define PF_W        (1 << 1)    /* Segment is writable */
#define PF_R        (1 << 2)    /* Segment is readable */
#define PF_MASKOS   0x0ff00000  /* OS-specific */
#define PF_MASKPROC 0xf0000000  /* Processor-specific */

/* Legal values for note segment descriptor types for core files. */

#define NT_PRSTATUS 1       /* Contains copy of prstatus struct */
#define NT_FPREGSET 2       /* Contains copy of fpregset struct */
#define NT_PRPSINFO 3       /* Contains copy of prpsinfo struct */
#define NT_PRXREG   4       /* Contains copy of prxregset struct */
#define NT_PLATFORM 5       /* String from sysinfo(SI_PLATFORM) */
#define NT_AUXV     6       /* Contains copy of auxv array */
#define NT_GWINDOWS 7       /* Contains copy of gwindows struct */
#define NT_ASRS     8       /* Contains copy of asrset struct */
#define NT_PSTATUS  10      /* Contains copy of pstatus struct */
#define NT_PSINFO   13      /* Contains copy of psinfo struct */
#define NT_PRCRED   14      /* Contains copy of prcred struct */
#define NT_UTSNAME  15      /* Contains copy of utsname struct */
#define NT_LWPSTATUS    16      /* Contains copy of lwpstatus struct */
#define NT_LWPSINFO 17      /* Contains copy of lwpinfo struct */
#define NT_PRFPXREG 20      /* Contains copy of fprxregset struct*/

/* Legal values for the note segment descriptor types for object files.  */

#define NT_VERSION  1       /* Contains a version string.  */

#endif