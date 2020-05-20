#pragma once

#ifdef __ASSEMBLER__

#define MULTIBOOT2_MAGIC    0xE85250D6 // Request
#define MULTIBOOT2_ARCH     0
#define MULTIBOOT2_LENGTH   (.multiboot_header_end - .multiboot_header_start)
#define MULTIBOOT2_CHECKSUM -(MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH + MULTIBOOT2_LENGTH)

#else

#include <boot/multiboot2_spec.h>
#include <mm/maddrs.h>
#include <stddef.h>

#define MULTIBOOT2_MAGIC               0x36D76289 // Reply

int multiboot2_init(uint64_t magic, void *data);

multiboot2_header_tag_t *multiboot2_get_tag(enum multiboot2_tag_type type);
multiboot2_header_tag_t *multiboot2_get_next_tag(void *current, enum multiboot2_tag_type type);

void multiboot2_get_mm_entries(multiboot2_header_tag_t *tag, multiboot2_mm_entry_tag_t **entries, uint32_t *number_entries);

#endif