LINKER_SCRIPT=linker.ld
INCLUDE_DIR=include/

CFLAGS:=$(CFLAGS) --sysroot=$(SYS_ROOT) -isystem=/usr/include
CFLAGS:=$(CFLAGS) -I$(INCLUDE_DIR)
CFLAGS:=$(CFLAGS) -O2 -g -std=gnu11 -ffreestanding -Wall -Wextra -nostdlib

KERNEL_OBJS=

.PHONY: all clean
.SUFFIXES: .o .c .S

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_OBJS) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $@ $(CFLAGS) $(LINK_LIST)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OBJS)
	rm -f $(KERNEL_OBJS:.o=.d)
