#include <amd64/asm/segment.h>
#include <amd64/asm/tss.h>
#include <amd64/cpu.h>
#include <assert.h>
#include <string.h>

void tss_init()
{
	struct tss_data *tss;

	tss = &(cpu_get()->tss);
	memset(tss, 0, sizeof(struct tss_data));
	tss->iomba = sizeof(struct tss_data);

	tss_install();
}

void tss_set_rsp(uint8_t index, void *rsp)
{
	struct tss_data *tss;

	// TODO: Make this value dynamic
	// Ensure that the index is a possible rspX value in the TSS
	ASSERT(index < 3);

	tss = &(cpu_get()->tss);
	tss->rsp[index] = (uint64_t)rsp;
}