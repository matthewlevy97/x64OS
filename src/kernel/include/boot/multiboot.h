#pragma once

#ifdef __ASSEMBLER__

#define MULTIBOOT2_MAGIC    0xE85250D6
#define MULTIBOOT2_ARCH     0x00000000
#define MULTIBOOT2_LENGTH   (.multiboot_header_end - .multiboot_header_start)
#define MULTIBOOT2_CHECKSUM -(MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH + MULTIBOOT2_LENGTH)

#else

/* Put C Code here*/
void multiboot_init();

#endif