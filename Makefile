# Flags for compiling
TARGET=x86_64-elf
CC=$(TARGET)-gcc
LD=$(TARGET)-ld
AS=$(TARGET)-as
AR=$(TARGET)-ar

CFLAGS += -std=c11 -Wall -Wextra -pedantic -ffreestanding -nostdlib
CPPFLAGS += -Iinclude
LDFLAGS +=
ASFLAGS +=

# Git Information
GITHASH := $(shell git log -1 --pretty="tformat:%h")
GITDATE := $(shell git log -1 --pretty="tformat:%cd")
GITBRANCH := $(shell git log -1 --pretty="tformat:%d" | tr -d '[:space:]')
GITFLAGS := \
   -DGITHASH='\"$(GITHASH)\"' \
   -DGITDATE='\"$(GITDATE)\"' \
   -DGITBRANCH='\"$(GITBRANCH)\"'
CFLAGS += $(GITFLAGS)

KERNEL_CFLAGS := $(CFLAGS)
KERNEL_CFLAGS += --sysroot=$(SYSROOT) -isystem=/usr/include -mcmodel=large
KERNEL_CFLAGS += -mno-red-zone -mno-mmx -mno-sse -mno-sse2
KERNEL_CPPFLAGS := $(CPPFLAGS)
KERNEL_LDFLAGS := $(LDFLAGS)
KERNEL_LDFLAGS :=  -n -nostdlib -lgcc
KERNEL_ASFLAGS := $(ASFLAGS)

# Enable Debug symbols?
DEBUG=1
ifdef DEBUG
KERNEL_CFLAGS += -O0 -ggdb -D_KERNEL_DEBUG
KERNEL_ASFLAGS += -ggdb
else
KERNEL_CFLAGS += -O3
endif

COMPILER_PROGRAMS:= \
	CC="$(CC)" \
	LD="$(LD)" \
	AS="$(AS)" \
	AR="$(AR)" \

COMPILE_FLAGS:= \
	$(COMPILER_PROGRAMS) \
	CFLAGS="$(CFLAGS)" \
	CPPFLAGS="$(CPPFLAGS)" \
	LDFLAGS="$(LDFLAGS)" \
	ASFLAGS="$(ASFLAGS)"

# Flags for compiling the kernel
KERNEL_COMPILE_FLAGS:= \
	$(COMPILER_PROGRAMS) \
	KERNEL_CFLAGS="$(KERNEL_CFLAGS)" \
	KERNEL_CPPFLAGS="$(KERNEL_CPPFLAGS)" \
	KERNEL_LDFLAGS="$(KERNEL_LDFLAGS)" \
	KERNEL_ASFLAGS="$(KERNEL_ASFLAGS)"

.PHONY: all sysroot clean mkiso kernel libc drivers apps ramdisk

all: sysroot libc kernel drivers apps ramdisk

sysroot:
	mkdir -p $(SYSROOT)

# Build libc and libk
libc:
	$(MAKE) -C src/libc install $(COMPILE_FLAGS) $(KERNEL_COMPILE_FLAGS)

# Build the kernel
kernel:
	$(MAKE) -C src/kernel install $(KERNEL_COMPILE_FLAGS)

drivers:
	$(MAKE) -C src/drivers install $(KERNEL_COMPILE_FLAGS)

apps:
	$(MAKE) -C src/apps install $(COMPILE_FLAGS)

ramdisk:
	@mkdir -p $(SYSROOT)/ramdisk/
	@echo "Adding files to initrd.img:"
	@cd $(SYSROOT)/ramdisk && tar cvf $(SYSROOT)/boot/initrd.img * 
	@rm -rf $(SYSROOT)/ramdisk/

mkiso: all

clean:
	$(MAKE) -C src/libc clean
	$(MAKE) -C src/kernel clean
	$(MAKE) -C src/apps clean
	$(MAKE) -C src/drivers clean
	rm -rf $(SYSROOT)
	rm -rf $(ISO)
