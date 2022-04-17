CFLAGS=-std=c90 -pedantic -D_POSIX_C_SOURCE=200112L -Wall -Wextra -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement

CFLAGS_FAST=$(CFLAGS) -O3 -DNDEBUG
CFLAGS_DEBUG=$(CFLAGS) -Og -g

CLINKS=-lm

DESTDIR=/usr/local

all: snowflake.o

snowflake.o: snowflake.c snowflake.h
	$(CC) $(CFLAGS_FAST) -o '$@' -c '$<'

snowflake_debug.o: snowflake.c snowflake.h
	$(CC) $(CFLAGS_DEBUG) -o '$@' -c '$<'

snowflake_test.o: snowflake_test.c snowflake.h
	$(CC) $(CFLAGS_DEBUG) -o '$@' -c '$<'

snowflake_test: snowflake_test.o snowflake_debug.o
	$(CC) $(CFLAGS_DEBUG) $(CLINKS) -o '$@' $^

libsnowflake.so: snowflake.c snowflake.h
	$(CC) $(CFLAGS_FAST) $(CLINKS) -shared -o '$@' -c '$<'

install: libsnowflake.so snowflake.h
	mkdir -p "$(DESTDIR)/include" "$(DESTDIR)/lib"
	cp snowflake.h "$(DESTDIR)/include"
	cp libsnowflake.so "$(DESTDIR)/lib"

uninstall:
	rm -f "$(DESTDIR)/include/snowflake.h"
	rm -f "$(DESTDIR)/lib/snowflake.so"

test: snowflake_test
	valgrind ./snowflake_test

clean:
	rm -f *.o

.PHONY: all install uninstall test clean
