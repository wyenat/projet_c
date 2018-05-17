CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g

all: recuperer_image zig_zag quantification

.PHONY: clean check

check:
	make -C tests/ check

clean:
	rm -f *~ *.o recuperer_image zig_zag quantification
