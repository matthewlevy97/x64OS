LINKER_SCRIPT=linker.ld
LIBC_DIRECTORY=../libc
KERNEL_BIN=kernel

CFLAGS := $(KERNEL_CFLAGS)
CPPFLAGS := $(KERNEL_CPPFLAGS) -I$(LIBC_DIRECTORY)/include
LDFLAGS := $(KERNEL_LDFLAGS) -T $(LINKER_SCRIPT)
ASFLAGS := $(KERNEL_ASFLAGS)

KERNEL_OBJS=
include boot/make.config
include acpi/make.config
include amd64/make.config
include drivers/make.config
include io/make.config
include make.config
include mm/make.config
include process/make.config
KERNEL_OBJS += $(SYSROOT)/usr/bin/libk.o

.PHONY: all install install-binary install-headers clean
.SUFFIXES: .o .c .S .S.py

all: $(KERNEL_BIN)

install: all install-binary install-headers

install-binary:
	@echo "Copying kernel to $(SYSROOT)/boot/$(KERNEL_BIN)"
	@mkdir -p $(SYSROOT)/boot
	@cp -a $(KERNEL_BIN) $(SYSROOT)/boot

install-headers:
	@mkdir -p $(SYSROOT)/usr/include/$(KERNEL_BIN)
	@cp -a include/* $(SYSROOT)/usr/include/$(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LINK.c) $^ -o $@

%.o: %.S.py
	python3 $^ | $(COMPILE.S) $(DEPFLAGS) -x assembler-with-cpp - -o $@

clean:
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OBJS)
