LINKER_SCRIPT=linker.ld
INCLUDE_DIR=include/

# Git Information
GITHASH := $(shell git log -1 --pretty="tformat:%h")
GITDATE := $(shell git log -1 --pretty="tformat:%cd")
GITBRANCH := $(shell git log -1 --pretty="tformat:%d" | tr -d '[:space:]')
GITFLAGS := \
   -DGITHASH='"$(GITHASH)"' \
   -DGITDATE='"$(GITDATE)"' \
   -DGITBRANCH='"$(GITBRANCH)"'

CFLAGS := -Wall -Wextra -pedantic
CFLAGS += --sysroot=$(SYSROOT) -isystem=/usr/include -mcmodel=large
CFLAGS += -std=c11 -ffreestanding -nostdlib
CFLAGS += -mno-red-zone -mno-mmx -mno-sse -mno-sse2
CFLAGS += $(GITFLAGS)

CPPFLAGS ?= -I$(INCLUDE_DIR)

LDFLAGS ?= -n -nostdlib -lgcc -T $(LINKER_SCRIPT)

# Enable Debug symbols?
DEBUG=1
ifdef DEBUG
CFLAGS += -O0 -ggdb -DKERNEL_DEBUG
ASFLAGS += -ggdb
else
CFLAGS += -O3
endif

KERNEL_OBJS=
include boot/make.config
include amd64/make.config
include acpi/make.config
include io/make.config
include libc/make.config
include mm/make.config
KERNEL_OBJS += \
	kmain.o \
	printk.o

.PHONY: all install clean
.SUFFIXES: .o .c .S .S.py

all: $(KERNEL_BIN)

install: all
	@echo "Copying kernel to $(SYSROOT)/boot/$(KERNEL_BIN)"
	@mkdir -p $(SYSROOT)/boot
	@cp -a $(KERNEL_BIN) $(SYSROOT)/boot

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LINK.c) $^ -o $@

%.o: %.S.py
	python3 $^ | $(COMPILE.S) $(DEPFLAGS) -x assembler-with-cpp - -o $@

clean:
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OBJS)
