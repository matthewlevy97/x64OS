#include <amd64/asm/gdt.h>
#include <amd64/cpu.h>
#include <kernel/debug.h>

static void fixup_tss_entry(struct segment_descriptor *tss_entry);

void gdt_init()
{
	struct gdt_page *gdt_page;
	struct desc_ptr *ptr;

	gdt_page = &(cpu_get()->gdt_page);
	ptr      = &(cpu_get()->gdt_ptr);

	fixup_tss_entry(&(gdt_page->gdt[GDT_ENTRY_TSS]));
	ptr->address = (uint64_t)&(gdt_page->gdt);

	gdt_reload(ptr);

}

static inline void fixup_tss_entry(struct segment_descriptor *tss_entry)
{
	struct segment_xdescriptor *tss_xentry;
	void *tss;

	tss_xentry = (struct segment_xdescriptor*)tss_entry;
	tss = &(cpu_get()->tss);

	tss_xentry->low.base0 = (uint16_t)(uintptr_t)tss;
	tss_xentry->low.base1 = ((uint64_t)(tss) >> 16) & 0xFF;
	tss_xentry->low.base2 = ((uint64_t)(tss) >> 24) & 0xFF;
	tss_xentry->base3 = (((uint64_t)(tss) >> 32) & 0xFFFFFFFFULL);
}