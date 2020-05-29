#include <drivers/elf.h>
#include <fs/vfs.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <mm/paging.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <process/scheduler.h>
#include <string.h>

static inline void map_data(process_t process, vfs_node_t file, struct elf_phdr64 ph);

// TODO: There are some serious lacks of input checking in this code

void elf_load()
{
	process_t           process;
	struct elf_header64 header;
	struct elf_phdr64   ph;
	vfs_node_t          file;
	uint64_t            offset;

	atomic_begin();

	process = get_current_process();

	file = kopen(process->name, 0);
	if(!file) {
		debug_warning("File not found!\n");
		goto load_exit;
	}

	vfs_read(file, 0, &header, sizeof(header));

	// Verify
	if(memcmp(header.e_ident.ei_magic, ELF_MAGIC, sizeof(header.e_ident.ei_magic))) {
		goto close_file_exit;
	}

	process->entry_point = (void (*)())header.e_entry;

	// Map in ELF
	offset = header.e_phoff;
	for(int i = header.e_phnum; i--;) {
		vfs_read(file, offset, &ph, sizeof(ph));
		
		if(ph.p_type == PT_LOAD)
			map_data(process, file, ph);

		offset += sizeof(ph);
	}
	
close_file_exit:
	vfs_close(file);
load_exit:
	atomic_end();
	return;
}

static void map_data(process_t process, vfs_node_t file, struct elf_phdr64 ph)
{
	uint64_t flags;
	uintptr_t vaddr;

	// Setup flags for pages
	flags = PAGE_PRESENT;
	if(!(ph.p_flags & PT_EXECUTABLE))
		flags |= PAGE_NX;
	if(ph.p_flags & PT_WRITABLE)
		flags |= PAGE_WRITE;

	vaddr = ALIGN_PAGE(ph.p_vaddr);
	for(uint64_t pages = 0; pages <= (ph.p_memsz / PAGE_SIZE); pages++, vaddr += PAGE_SIZE) {
		vmm_map_page2((uintptr_t)pmm_calloc, vaddr, flags);
	}

	vfs_read(file, ph.p_offset, (void*)ALIGN_PAGE(ph.p_vaddr), ph.p_filesz);

	// Note: If the kernel CPL=0 writes to a user page, the page loses its USER-bit. Need to remap
	if(!(process->kernel_mode)) {
		flags |= PAGE_USER;
		vaddr = ALIGN_PAGE(ph.p_vaddr);
		for(uint64_t pages = 0; pages <= (ph.p_memsz / PAGE_SIZE); pages++, vaddr += PAGE_SIZE) {
			vmm_set_page_flags(vaddr, flags);
		}
	}
}