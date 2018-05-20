CC=clang
CFLAGS=-std=c99 -Wall -Wextra -Wabsolute-value -g -lm

all:  parser ACDC hex

parser: parser.o recuperer_image.o decoupage_MCU.o jpeg_writer_prof.o RGB2YCbCr.o zig_zag.o DCT.o quantification.o
	$(CC) -o parser recuperer_image.o parser.o decoupage_MCU.o RGB2YCbCr.o zig_zag.o DCT.o quantification.o $(CFLAGS)
 
parser.o: parser.c recuperer_image.h decoupage_MCU.h RGB2YCbCr.h zig_zag.h DCT.h
	$(CC) -o parser.o -c parser.c $(CFLAGS)

quantification.o: quantification.h ../include/qtables.h quantification.c
	$(CC) -o quantification.o -c quantification.c $(CFLAGS)


DCT.o: DCT.h decoupage_MCU.h recuperer_image.h DCT.c 
	$(CC) -o DCT.o -c DCT.c $(CFLAGS)

zig_zag.o: decoupage_MCU.h zig_zag.h zig_zag.c
	$(CC) -o zig_zag.o -c zig_zag.c $(CFLAGS)

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
