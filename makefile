icnlude config.mk

CC=cc

.PHONY: all install clean remove

all: build/liblog.so.$(VERSION)

build/liblog.so.$(VERSION): $(OBJS)
	$(CC) $(OBJS) -shared -Wl,-soname,liblog.so -o $@

build/log.o: src/log.c
	$(CC) -fpic -o $@ -c $<

install: all
	sudo install build/liblog.so.$(VERSION) $(PREFIX)/lib
	sudo ln -s $(PREFIX)/lib/liblog.so.$(VERSION) $(PREFIX)/lib/liblog.so

clean:
	rm -f $(OBJS) build/liblog.so.$(VERSION)

remove:
	sudo rm -rf $(PREFIX)/include/log
	sudo rm -f $(PREFIX)/lib/liblog.so.$(VERSION) $(PREFIX)/lib/liblog.so
