#pragma once

#include <amd64/asm/segment.h>

typedef struct {
	struct desc_ptr gdt_ptr;
	struct gdt_page gdt_page;
	
	struct tss_data tss;
} cpu_t;

void cpu_init();

cpu_t *cpu_get();