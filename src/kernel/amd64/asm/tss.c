#include <amd64/asm/segment.h>
#include <amd64/asm/tss.h>
#include <amd64/cpu.h>
#include <string.h>

void tss_init()
{
	struct tss_data *tss;

	tss = &(cpu_get()->tss);
	memset(tss, 0, sizeof(struct tss_data));
	tss->iomba = sizeof(struct tss_data);

	tss_install();
}

void tss_set_rsp0(void *rsp0)
{
	struct tss_data *tss;

	tss = &(cpu_get()->tss);
	tss->rsp[0] = (uint64_t)rsp0;
}