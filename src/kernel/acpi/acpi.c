#include <acpi/acpi.h>
#include <boot/multiboot2.h>
#include <mm/paging.h>
#include <stdbool.h>
#include <string.h>

static void parse_acpi(struct RSDPDescriptor *rsdp);
static void parse_acpiv2(struct RSDPDescriptor20 *rsdp);

static bool is_rsdp_valid(struct RSDPDescriptor *rsdp);
static uint8_t verify_checksum(void *bytes, uint32_t length);

static void parse_fadt(struct ACPISDTHeader *header);

void acpi_init()
{
	multiboot2_rsdp_header_tag_t *tag;
	
	tag = (multiboot2_rsdp_header_tag_t*)multiboot2_get_tag(MBOOT2_ACPIv2);
	if(tag) {
		parse_acpiv2((struct RSDPDescriptor20*)&(tag->rsdp));
		return;
	}
	
	tag = (multiboot2_rsdp_header_tag_t*)multiboot2_get_tag(MBOOT2_ACPI);
	if(tag) {
		parse_acpi((struct RSDPDescriptor*)&(tag->rsdp));
		return;
	}
}

static void parse_acpi(struct RSDPDescriptor *rsdp)
{
	struct ACPISDTHeader *header;
	struct RSDT *rsdt;
	uint32_t num_entries;
	
	// Sanity checks
	if(!is_rsdp_valid(rsdp))
		return;
	
	rsdt = (struct RSDT*)P2V(rsdp->rsdt_address);
	num_entries = (rsdt->header.length - sizeof(rsdt->header)) / sizeof(uint32_t);
	
	for(uint32_t i = 0; i < num_entries; i++) {
		header = (struct ACPISDTHeader*)P2V(rsdt->entries[i]);
		
		// Validate table
		if(verify_checksum(header, header->length)) {
			// XXX: Invalid ACPI table
			continue;
		}


		if(!strncmp(header->signature, "FACP", sizeof(header->signature))) {
			parse_fadt(header);
		}
	}
	
}

static void parse_acpiv2(struct RSDPDescriptor20 *rsdp)
{	
	// TODO: Parse ACPIv2
}

static bool is_rsdp_valid(struct RSDPDescriptor *rsdp)
{
	if(rsdp->revision != 0 || verify_checksum(rsdp, sizeof(struct RSDPDescriptor)) ||
		strncmp(rsdp->signature, RSDP_SIGNATURE, sizeof(RSDP_SIGNATURE) - 1))
		return false;
	
	return true;
}

static uint8_t verify_checksum(void *bytes, uint32_t length)
{
	char *ptr;
	uint8_t checksum;
	
	ptr      = bytes;
	checksum = 0;
	
	while(length--) {
		checksum += *ptr++;
	}
	
	// Checksum == 0 means valid
	return checksum;
}

static void parse_fadt(struct ACPISDTHeader *header)
{
	struct FADT *fadt;
	struct FACS *facs;
	struct DSDT *dsdt;

	// XXX: Actually parse the FADT table from ACPI
	fadt = (struct FADT*)header;
	facs = P2V(fadt->firmware_ctrl);
	dsdt = P2V(fadt->dsdt);
}