.PHONY: all test lint clean install
TARGETS = /usr/local/lib/liblog.so /usr/local/lib/liblog.a /usr/local/include/log.h
all: $(TARGETS)

liblog.so: src/log.c src/log.h
	gcc -g -c -Wall -Wextra -fPIC -O0 -o log-pic.o src/log.c
	g++ -g -fPIC -rdynamic -shared -o $@ log-pic.o

liblog_asan.so: src/log.c src/log.h
	gcc -g -c -Wall -Wextra -fsanitize=address,undefined -fPIC -O0 -o log_asan.o src/log.c
	g++ -g -fsanitize=address,undefined -fPIC -rdynamic -shared -o $@ log_asan.o

liblog.a: src/log.c src/log.h
	gcc -g -c -Wall -Wextra -O0 -o log.o src/log.c
	ar crf $@ log.o

test: test/Makefile test/test.c liblog_asan.so lint
	make -C test test

lint:
	-clang-tidy src/log.c src/log.h
	cppcheck -Isrc src/log.c src/log.h

/usr/local/lib/liblog.so: liblog.so
	sudo cp liblog.so /usr/local/lib/
/usr/local/lib/liblog_asan.so: liblog_asan.so
	sudo cp liblog_asan.so /usr/local/lib/
/usr/local/lib/liblog.a: liblog.a
	sudo cp liblog.a  /usr/local/lib/
/usr/local/include/log.h:
	sudo cp src/log.h /usr/local/include/

install: $(TARGETS)

clean:
	rm liblog.so liblog.a log.o liblog_asan.so log_asan.o
	sudo rm /usr/local/lib/liblog.so /usr/local/lib/liblog.a /usr/local/lib/liblog_asan.so \
                /usr/local/include/log.h
