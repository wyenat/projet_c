#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex.h"
#include "bitstream_perso.h"

struct bitstream *bitstream_create(const char *filename){
  FILE *file;
  struct bitstream *bts = malloc(sizeof(struct bitstream));
  file = fopen(filename, "w+");
  bts->file = file;
  bts->buffer = 0;
  bts->compteur = 0;
  bts->dernier_ecrit = 0;
  return bts;
}

void byte_stuffing(struct bitstream *stream){
  uint32_t zero = 0;
  fwrite(&zero, 1, 1, stream->file);
  stream->dernier_ecrit = 0;
}

uint32_t create_mask(uint8_t nbits){
  uint32_t mask = 0;
   for (int i=0; i<nbits; i++){
     mask |= 1 << i;
   }
   return mask;
}

void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker){
  if (is_marker == 1 && stream->buffer != 0 && stream->buffer != 1){
    // printf("Value = %s, bitflush !\n", hexme(value));
    bitstream_flush(stream);
  }
  uint32_t mask = create_mask(nbits);
  stream->compteur += nbits;
  stream->buffer = stream->buffer << (nbits);
  value = value & mask;
  uint8_t taille = stream->compteur / 8;
  uint8_t reste = stream->compteur % 8;
  stream->buffer |= value;
  // printf("Taille = %d \n", taille);
  uint32_t a_ecrire = stream->buffer >> reste;
  uint32_t mask8 = create_mask(8);
  uint32_t a_ecrire_8 = 0;
  while (taille > 0){
    a_ecrire_8 = (a_ecrire >> (8*(taille - 1))) & mask8;
    stream->dernier_ecrit = stream->dernier_ecrit << 8;
    fwrite(&a_ecrire_8, 1, 1, stream->file);
    stream->compteur -= 8;
    taille -= 1;
    stream->dernier_ecrit += a_ecrire_8;
    if (stream->dernier_ecrit == 255 && is_marker==0){
      // printf("BYTE STUFFING !\n");
      byte_stuffing(stream);
    }
  }
  mask = create_mask(reste);
  stream->buffer &= mask;
}

void bitstream_flush(struct bitstream *stream){
  uint8_t reste = stream->compteur;
  // printf("Reste à %d \n", reste);
  if (reste == 0){
    // printf(" Rien à flush \n ");
  } else {
    // printf("On rajoute %d zéros \n", 8-reste);
      // stream->buffer = stream->buffer << ((8-reste)%8);
      // printf("compteur = %d \n", stream->compteur);
      fwrite(&stream->buffer, 1, 1, stream->file);
  }
  stream->compteur = 0;
  stream->buffer = 0;
  stream->dernier_ecrit = 0; //Peu importe la valeur écrite, ce ne sera pas un f, donc pas de ff à venir.
}

void bitstream_destroy(struct bitstream *stream){
  fclose(stream->file);
  free(stream);
}
