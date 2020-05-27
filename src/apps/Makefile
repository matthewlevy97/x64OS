.PHONY: all clean install build-apps

all: install-apps build-apps

install: all

APPLICATIONS:=
include make.config

# Build the apps
build-apps: $(APPLICATIONS)
	$(MAKE) -C "$<" install $(COMPILE_FLAGS)

install-apps:
	@mkdir -p $(SYSROOT)/ramdisk/apps

clean: $(APPLICATIONS)
	$(MAKE) -C "$<" clean $(KERNEL_COMPILE_FLAGS)
