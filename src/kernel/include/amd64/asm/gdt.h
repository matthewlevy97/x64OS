#pragma once

#include <amd64/asm/segment.h>

void gdt_init();
void gdt_reload(struct desc_ptr *desc_ptr);