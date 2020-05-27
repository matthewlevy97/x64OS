#pragma once

/* https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html */

#include <stdint.h>

#if 0
 	+---------------+ 
 	| stack pointer | grows toward lower addresses
 	+---------------+ ||
 	|...............| \/
 	|...............|
 	|...............|
 	|...............| /\
 	+---------------+ ||
 	|  brk (heap)   | grows toward higher addresses
 	+---------------+
 	| .bss section  |
 	+---------------+
 	| .data section |
 	+---------------+
 	| .text section |
 	+---------------+
#endif

#define ELF_MAGIC "\x7F""ELF"

struct elf_ident {
	char ei_magic[4];
	uint8_t ei_class;
	uint8_t ei_data;
	uint8_t ei_version;
	uint8_t ei_osabi;
	uint8_t ei_abiversion;
	uint8_t _padding[7];
} __attribute__((packed));

struct elf_header64 {
	struct elf_ident e_ident;
	
	uint16_t   e_type;
	uint16_t   e_machine;
	uint32_t   e_version;
	
	uintptr_t  e_entry;
	uintptr_t  e_phoff;
	uintptr_t  e_shoff;
	
	uint32_t   e_flags;
	uint16_t   e_ehsize;
	uint16_t   e_phentsize;
	uint16_t   e_phnum;
	uint16_t   e_shentsize;
	uint16_t   e_shnum;
	uint16_t   e_shstrndx;
} __attribute__((packed));

typedef enum {
	PT_NULL,
	PT_LOAD,
	PT_DYNAMIC,
	PT_INTERP,
	PT_NOTES
} PH_TYPE;
typedef enum {
	PT_EXECUTABLE = 1,
	PT_WRITABLE   = 2,
	PT_READABLE   = 4
} PH_FLAGS;

struct elf_phdr64 {
	uint32_t   p_type;
	uint32_t   p_flags;
	
	uintptr_t  p_offset;
	uintptr_t  p_vaddr;
	uintptr_t  p_paddr;
	
	uint64_t   p_filesz;
	uint64_t   p_memsz;
	uint64_t   __padding;
} __attribute__((packed));

void elf_load();