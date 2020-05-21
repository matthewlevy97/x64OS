#include <boot/multiboot2.h>
#include <kernel/kernel.h>
#include <kernel/debug.h>

static multiboot2_header_tag_t *mboot2_address;

int multiboot2_init(uint64_t magic, void *data)
{
	if(magic != MULTIBOOT2_MAGIC)
		return 0;
	
	mboot2_address = (multiboot2_header_tag_t*)data;

	return 1;
}

multiboot2_header_tag_t *multiboot2_get_tag(enum multiboot2_tag_type type)
{
	return multiboot2_get_next_tag(NULL, type);
}

multiboot2_header_tag_t *multiboot2_get_next_tag(void *current, enum multiboot2_tag_type type)
{
	multiboot2_header_tag_t *header, *current_tag;
	uint32_t total_length;
	
	header      = mboot2_address;
	current_tag = (multiboot2_header_tag_t*)current;

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
		if(!current_tag && header->type == type)
			goto return_tag;

		if(header == current_tag) {
			current_tag = NULL;
		}

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

void multiboot2_information(uintptr_t *base_address, uint32_t *length)
{
	*base_address = (uintptr_t)mboot2_address;
	*length = *(uint32_t*)mboot2_address;
}