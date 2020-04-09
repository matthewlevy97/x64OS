# Git version information
#GITHASH := $(shell git log -1 --pretty="tformat:%h")
#GITDATE := $(shell git log -1 --pretty="tformat:%cd")
#GITDIRTY := $(shell git status -s >/dev/null 2>/dev/null && echo 1 || echo 0)
#GITMESSAGE := $(shell git log -1 --pretty="tformat:%s")
#GITBRANCH := $(shell git log -1 --pretty="tformat:%d")
#GITFLAGS := \
#	-DGITHASH='"$(GITHASH)"' \
#	-DGITDATE='"$(GITDATE)"' \
#	-DGITDIRTY='$(GITDIRTY)' \
#	-DGITMESSAGE='"$(GITMESSAGE)"' \
#	-DGITBRANCH='"$(GITBRANCH)"'

# Flags for compiling
TARGET=x86_64-elf
CC=$(TARGET)-gcc
LD=$(TARGET)-ld
AS=$(TARGET)-as
AR=$(TARGET)-ar
COMPILE_FLAGS:= \
	CC="$(CC)" \
	LD="$(LD)" \
	AS="$(AS)" \
	AR="$(AR)"

# Flags for compiling the kernel
KERNEL_COMPILE_FLAGS:= \
	$(COMPILE_FLAGS) \
	KERNEL_BIN="kernel"

.PHONY: all sysroot clean mkiso kernel

all: sysroot kernel

sysroot:
	mkdir -p $(SYSROOT)

# Build the kernel
kernel:
	$(MAKE) -C src/kernel install $(KERNEL_COMPILE_FLAGS)

mkiso: all
	

clean:
	$(MAKE) -C src/kernel clean $(KERNEL_COMPILE_FLAGS)
	rm -rf $(SYSROOT)
	rm -rf $(ISO)
