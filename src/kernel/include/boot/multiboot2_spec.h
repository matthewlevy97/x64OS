#pragma once

#include <stdint.h>

enum multiboot2_tag_type {
	MBOOT2_MM = 6,
};

typedef struct {
	union {
		uint32_t _type;
		struct {
			uint16_t type;
			uint16_t flags;
		};
	};
	uint32_t size;
} multiboot2_header_tag_t;

#define MBOOT2_MM_AVAILABLE   1
#define MBOOT2_MM_ACPI        3
#define MBOOT2_MM_HIBERNATION 4
#define MBOOT2_MM_DEFECTIVE   5
typedef struct {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
	uint32_t _;
} multiboot2_mm_entry_tag_t;
typedef struct {
	multiboot2_header_tag_t header;
	uint32_t entry_size;
	uint32_t version;
	multiboot2_mm_entry_tag_t entries[];
} multiboot2_mm_header_tag_t;