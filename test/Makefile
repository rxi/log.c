
test: test.c Makefile ../liblog_asan.so
	gcc -Wall -Wextra -fsanitize=address,undefined test.c -I../src ../liblog_asan.so -o test
	LD_LIBRARY_PATH=.. ./test

../liblog_asan.so: ../src/log.c ../src/log.h
	$(MAKE) -C .. liblog_asan.so

clean:
	rm my_log test liblog_asan.so
