OBJS=test.o

.PHONY: install clean

install: test_app
	@echo "Installing test application: $(SYSROOT)/ramdisk/apps/test_app"
	@cp -a test_app "$(SYSROOT)/ramdisk/apps/test_app"

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

test_app: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(OBJS)
	rm -f "$(SYSROOT)/ramdisk/apps/test_app"