#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitstream_perso.h"

struct bitstream *bitstream_create(const char *filename){
  FILE *file;
  struct bitstream *bts = malloc(sizeof(struct bitstream));
  file = fopen(filename, "w+");
  bts->file = file;
  bts->buffer = 0;
  bts->compteur = 0;
  bts->deux_dernier_bytes_ecrits = 0;
  return bts;
}

void byte_stuffing(struct bitstream *stream){
  fwrite(0, 2, 1, stream->file);
  stream->deux_dernier_bytes_ecrits = 0;
}

uint32_t create_mask(uint8_t nbits){
  uint32_t mask = 0;
   for (int i=0; i<nbits; i++){
     mask |= 1 << i;
   }
   return mask;
}

void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker){
  if (is_marker == 1){
    bitstream_flush(stream);
  }
  uint32_t mask = create_mask(nbits);
  stream->compteur += nbits;
  stream->buffer = stream->buffer << nbits;
  value = value & mask;
  uint8_t taille = stream->compteur / 8;
  uint8_t reste = stream->compteur % 8;   
  stream->buffer |= value;
  printf("Taille = %d \n", taille);
  uint32_t a_ecrire = stream->buffer >> reste;
  uint32_t mask8 = create_mask(8);
  uint32_t a_ecrire_8 = a_ecrire & mask8; 
  while (taille > 0){
    stream->deux_dernier_bytes_ecrits = stream->deux_dernier_bytes_ecrits << 8;
    fwrite(&a_ecrire_8, 1, 1, stream->file);
    stream->compteur -= 8;
    taille -= 1;
    a_ecrire = a_ecrire >> 8;
    a_ecrire_8 = a_ecrire & mask8; 
    stream->deux_dernier_bytes_ecrits += a_ecrire_8;
    if (stream->deux_dernier_bytes_ecrits == -1){
      byte_stuffing(stream);
    }
  }
  mask = create_mask(reste);
  stream->buffer &= mask;
}

void bitstream_flush(struct bitstream *stream){
  uint8_t reste = stream->compteur % 8;  
  printf("Reste à %d \n", reste);
  if (reste == 0){
    printf(" Rien à flush \n ");
  } else {
    // printf("On rajoute %d zéros \n", 8-reste);
      stream->buffer = stream->buffer << (8-reste);
      stream->compteur++;  
      printf("compteur = %d \n", stream->compteur);
      bitstream_write_nbits(stream, stream->buffer, 0, 0);
  }
}

void bitstream_destroy(struct bitstream *stream){
  free(stream->file);
  free(stream);
}

  int main(){
    struct bitstream *bts = bitstream_create("Boulii");
    printf("Noue !! \n");
    for (int i=1; i<6; i++){
      // printf("%d \n", i);
      bitstream_write_nbits(bts, i, 3, 0);
    }
    bitstream_flush(bts);
    bitstream_flush(bts);
    // bitstream_flush(bts);
    // bitstream_destroy(bts);
  }