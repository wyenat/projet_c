CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g -lm

all:  parser #decoupage_MCU

parser: parser.o recuperer_image.o decoupage_MCU.o jpeg_writer_prof.o RGB2YCbCr.o
		$(CC) -o parser recuperer_image.o parser.o decoupage_MCU.o RGB2YCbCr.o

parser.o: parser.c recuperer_image.h decoupage_MCU.h RGB2YCbCr.h
		$(CC) -o parser.o -c parser.c $(CFLAGS)

# decoupage_MCU: recuperer_image.o decoupage_MCU.o
# 		$(CC) -o decoupage_MCU recuperer_image.o decoupage_MCU.o

jpeg_writer_prof.o: jpeg_writer_prof.h

recuperer_image.o: recuperer_image.c
	  $(CC) -o recuperer_image.o -c recuperer_image.c $(CFLAGS)

decoupage_MCU.o: recuperer_image.h decoupage_MCU.c
	  $(CC) -o decoupage_MCU.o -c decoupage_MCU.c $(CFLAGS)

RGB2YCbCr.o: decoupage_MCU.h RGB2YCbCr.c
	$(CC) -o RGB2YCbCr.o -c RGB2YCbCr.c $(CFLAGS)



.PHONY: clean check

check:
	make -C tests/ check

clean:
	rm -f *~ *.o recuperer_image RGB2YCbCr decoupage_MCU parser
