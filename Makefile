# Git version information
GITHASH := $(shell git log -1 --pretty="tformat:%h")
GITDATE := $(shell git log -1 --pretty="tformat:%cd")
GITDIRTY := $(shell git status -s >/dev/null 2>/dev/null && echo 1 || echo 0)
GITMESSAGE := $(shell git log -1 --pretty="tformat:%s")
GITBRANCH := $(shell git log -1 --pretty="tformat:%d")
GITFLAGS := \
	-DGITHASH='"$(GITHASH)"' \
	-DGITDATE='"$(GITDATE)"' \
	-DGITDIRTY='$(GITDIRTY)' \
	-DGITMESSAGE='"$(GITMESSAGE)"' \
	-DGITBRANCH='"$(GITBRANCH)"'

# Flags for compiling
TARGET=x86_64-elf
CC=$(TARGET)-gcc
AR=$(TARGET)-ar
LD=$(TARGET)-ld
CFLAGS=
LFLAGS=
COMPILE_FLAGS:= \
	CC=$(CC) \
	AR=$(AR) \
	LD=$(LD) \
	CFLAGS=$(CFLAGS) \
	LFLAGS=$(LFLAGS)

# System root
SYS_ROOT=sysroot

# Flags for compiling the kernel
KERNEL_BIN=kernel.bin
KERNEL_COMPILE_FLAGS:= \
	$(COMPILE_FLAGS) \
	KERNEL_BIN=$(KERNEL_BIN)

all: kernel

# Build the kernel
kernel:
	$(MAKE) -C src/kernel all $(KERNEL_COMPILE_FLAGS) SYS_ROOT=$(SYS_ROOT)

clean:
	$(MAKE) -C src/kernel clean KERNEL_BIN=$(KERNEL_BIN)
