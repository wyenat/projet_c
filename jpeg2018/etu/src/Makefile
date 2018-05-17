CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g

all: recuperer_image quantification RBG2YCbCr

.PHONY: clean check

check:
	make -C tests/ check

clean:
	rm -f *~ *.o recuperer_image quantification RGB2YCbCr
