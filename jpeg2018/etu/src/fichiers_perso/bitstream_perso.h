#ifndef _BITSTREAM_PERSO_H_
#define _BITSTREAM_PERSO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bitstream{
  FILE *file;
  uint8_t compteur;
  uint32_t buffer;
  uint8_t dernier_ecrit;
};

/* Alloue et renvoie un bitstream qui écrit dans le fichier de nom filename */
struct bitstream *bitstream_create(const char *filename);

/* Si on écrit FF, on écrit 00 derrière */
void byte_stuffing(struct bitstream *stream);

/* Crée un masque de taille nbits */
uint32_t create_mask(uint8_t nbits);

/* Ecrit dans le bitstream stream la valeur contenue dans value, sur nbits.
is_marker vaut 1 si on écrit dans l'entête jpeg, 0 sinon */
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker);

/* Force l'écriture des bits en attente, en mettant des 0 à droite pour completer */
void bitstream_flush(struct bitstream *stream);

/* Détruit le bitstream, libère la mémoire */
void bitstream_destroy(struct bitstream *stream);

#endif /* _BITSTREAM_PERSO_H_ */

