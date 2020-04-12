#include <boot/multiboot2.h>
#include <kernel/kernel.h>
#include <string.h>

int multiboot2_init(uint64_t magic, void *data)
{
	if(magic != MULTIBOOT2_MAGIC)
		return 0;

	// Relocate MBoot data to 0xFFFFFF8000000000
	memcpy((void*)MULTIBOOT2_RELOCATION_ADDRESS, data, *(uint32_t*)data);

	return 1;
}

multiboot2_header_tag_t *multiboot2_get_tag(enum multiboot2_tag_type type)
{
	multiboot2_header_tag_t *header;
	uint32_t total_length;
	
	header = (multiboot2_header_tag_t*)MULTIBOOT2_RELOCATION_ADDRESS;

	/**
	 * The first tag has the following struct
	 * {
	 *	uint32_t size;
	 *	uint32_t reserved;
	 * }
	 * 
	 * We will use the header structure as it has the same layout with different names
	 * Note: This assumes that the bootloader's returned value for total size is not < sizeof(total_length)
	 */
	total_length = header->_type - sizeof(total_length);

	header = incptr(header, sizeof(header));
	while(total_length && header->type) {
		if(header->type == type)
			goto return_tag;

		total_length -= ALIGN_8(header->size);
		header = incptr(header, ALIGN_8(header->size));
	}
	header = NULL;

return_tag:
	return header;
}

void multiboot2_get_mm_entries(multiboot2_header_tag_t *tag, multiboot2_mm_entry_tag_t **entries, uint32_t *number_entries)
{
	multiboot2_mm_header_tag_t *header;

	header = (multiboot2_mm_header_tag_t*)tag;

	if(header->version != 0) {
		// XXX: Only supports version 0 of Memory Map entries
		*entries        = NULL;
		*number_entries = 0;
		return;
	}

	*entries = header->entries;
	*number_entries = (header->header.size - sizeof(*header)) / header->entry_size;
}