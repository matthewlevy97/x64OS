.PHONY: all clean install build-drivers

all: install-drivers build-drivers

install: all

DRIVERS:=
include make.config

# Build the drivers
build-drivers: $(DRIVERS)
	$(MAKE) -C "$<" install $(KERNEL_COMPILE_FLAGS)

install-drivers:
	@mkdir -p $(SYSROOT)/ramdisk/drivers

clean: $(DRIVERS)
	$(MAKE) -C "$<" clean $(KERNEL_COMPILE_FLAGS)
