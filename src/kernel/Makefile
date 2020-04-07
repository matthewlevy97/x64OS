LINKER_SCRIPT=linker.ld
INCLUDE_DIR=include/

CFLAGS += --sysroot=$(SYSROOT) -isystem=/usr/include
CFLAGS += -std=gnu11 -ffreestanding -nostdlib
CFLAGS += -mno-red-zone -mno-mmx -mno-sse -mno-sse2

CPPFLAGS += -I$(INCLUDE_DIR)

LDFLAGS += -n -nostdlib -lgcc -T $(LINKER_SCRIPT)

# Enable Debug symbols?
ifdef DEBUG
CFLAGS += -O0 -ggdb -DDEBUG
ASFLAGS += -ggdb
else
CFLAGS += -O3
endif

include boot/make.config
KERNEL_OBJS+=\
	kmain.o

.PHONY: all clean install
.SUFFIXES: .o .c .S

all: $(KERNEL_BIN)

install: all
	@echo "Copying kernel to $(SYSROOT)/boot/$(KERNEL_BIN)"
	@mkdir -p $(SYSROOT)/boot
	@cp -a $(KERNEL_BIN) $(SYSROOT)/boot

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LINK.c) $^ -o $@

clean:
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OBJS)