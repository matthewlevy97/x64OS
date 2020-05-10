#pragma once

#include <stdint.h>

#define RSDP_SIGNATURE "RSD PTR "

struct RSDPDescriptor {
	char     signature[8];
	uint8_t  checksum;
	char     OEMID[6];
	uint8_t  revision;
	uint32_t rsdt_address;
} __attribute__ ((packed));

struct RSDPDescriptor20 {
	struct RSDPDescriptor v1;

	uint32_t length;
	uint64_t xsdt_address;
	uint8_t  extended_checksum;
	uint8_t  _[3];
} __attribute__ ((packed));

struct ACPISDTHeader {
  char     signature[4];
  uint32_t length;
  uint8_t  revision;
  uint8_t  checksum;
  char     OEMID[6];
  char     OEM_table_id[8];
  uint32_t OEM_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__ ((packed));

struct RSDT {
  struct ACPISDTHeader header;
  uint32_t entries[];
} __attribute__ ((packed));

struct XSDT {
  struct ACPISDTHeader header;
  uint64_t entries[];
} __attribute__ ((packed));

struct GenericACPIAddress {
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
} __attribute__ ((packed));

struct FADT {
  struct ACPISDTHeader header;
  
  uint32_t firmware_ctrl;
  uint32_t dsdt;
  uint8_t _reserved_44;
  uint8_t prefered_pm_profile;
  
  uint16_t sci_int;
  uint32_t smi_cmd;
  
  uint8_t acpi_enable;
  uint8_t acpi_disable;
  
  uint8_t s4bios_req;
  uint8_t pstate_cnt;
  
  uint32_t pm1a_evt_blk;
  uint32_t pm1b_evt_blk;
  
  uint32_t pm1a_cnt_blk;
  uint32_t pm1b_cnt_blk;
  
  uint32_t pm2_cnt_blk;
  
  uint32_t pm_tmr_blk;
  
  uint32_t gpe0_blk;
  uint32_t gpe1_blk;
  
  uint8_t pm1_evt_len;
  uint8_t pm1_cnt_len;
  
  uint8_t pm2_cnt_len;
  
  uint8_t pm_tmr_len;
  
  uint8_t gpe0_blk_len;
  uint8_t gpe1_blk_len;
  uint8_t gpe1_base;
  
  uint8_t cst_len;
  
  uint16_t p_lvl2_lat;
  uint16_t p_lvl3_lat;
  
  uint16_t flush_size;
  uint16_t flush_stride;
  
  uint8_t duty_offset;
  uint8_t duty_width;
  
  uint8_t day_alrm;
  uint8_t mon_alrm;
  uint8_t century;
  
  uint16_t iapc_boot_arch;
  
  uint8_t _reserved_111;
  
  uint32_t flags;
  struct GenericACPIAddress reset_reg;
  uint8_t reset_value;
  
  uint16_t arm_boot_arch;
  uint8_t fadt_minor_version;
  
  uint64_t x_firmware_ctrl;
  uint64_t x_dsdt;
  
  struct GenericACPIAddress x_pm1a_evt_blk;
  struct GenericACPIAddress x_pm1b_evt_blk;
  
  struct GenericACPIAddress x_pm1a_cnt_blk;
  struct GenericACPIAddress x_pm1b_cnt_blk;
  
  struct GenericACPIAddress x_pm2_cnt_blk;
  
  struct GenericACPIAddress x_pm_tmr_blk;
  
  struct GenericACPIAddress x_gpe0_blk;
  struct GenericACPIAddress x_gpe1_blk;
  
  struct GenericACPIAddress sleep_control_reg;
  struct GenericACPIAddress sleep_status_reg;
  
  uint64_t hypervisor_identity;
} __attribute__ ((packed));


struct FACS {
  char signature[4];
  uint32_t length;
  uint32_t hardware_signature;
  uint32_t firmware_waking_vector;
  uint32_t global_lock;
  uint32_t flags;
  uint64_t x_firmware_waking_vector;
  uint8_t version;
  uint8_t _reserved_33[3];
  uint32_t ospm_flags;
  uint8_t _reserved_40[24];
} __attribute__ ((packed));

struct DSDT {
  struct ACPISDTHeader header;
  uint8_t aml[];
} __attribute__ ((packed));

void acpi_init();