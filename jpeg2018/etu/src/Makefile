CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g

all: recuperer_image RBG2YCbCr decoupage_MCU parser

decoupage_MCU: recuperer_image.o decoupage_MCU.o
						$(CC) -o decoupage_MCU recuperer_image.o decoupage_MCU.o

recuperer_image.o: recuperer_image.c
	$(CC) -o recuperer_image.o -c recuperer_image.c $(CFLAGS)

decoupage_MCU.o: recuperer_image.h decoupage_MCU.o
	   $(CC) -o decoupage_MCU.o -c decoupage_MCU.c $(CFLAGS)



.PHONY: clean check

check:
	make -C tests/ check

clean:
	rm -f *~ *.o recuperer_image RGB2YCbCr decoupage_MCU parser
