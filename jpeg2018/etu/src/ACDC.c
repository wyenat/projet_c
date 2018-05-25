#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"
#include "DCT.h"
#include "bitstream.h"
#include "huffman.h"
#include "jpeg_writer.h"

//Je ne comprends pas bien comment gerer le byte stuffing mais sinon tout fonctionne.
//Tout le module ne fait pour l'instant que de l'affichage, il sortira des char* ou
//des ints par la suite selon ce que le bitstream prendra

// Partie magnétude, indice
uint32_t  obtenir_magnetude(int16_t entree){
  if (entree == 0){
    return 0;
  }
  int32_t  magnetude = log2(abs(entree))+1;
  return magnetude;
}

uint32_t  obtenir_indice(int16_t entree, int32_t  magnetude){
  if (magnetude == 0){
    return 0;
  }
  uint32_t indice = abs(entree);
  if (entree < 0){
    indice = pow(2, magnetude) - indice - 1;
  }
  return indice;
}

void affichage_ind_magn(int16_t entree){
  uint32_t  magnetude = obtenir_magnetude(entree);
  uint32_t  indice = obtenir_indice(entree, magnetude);
  printf("\n Le nombre %d a pour magnétude %d, pour indice %d \n", entree, magnetude, indice);
}

// Partie symbole symbole LRE
int compter_zero(int16_t *entree, int *courant){
  int nb_zero = 0;
  while (*courant < 64 && entree[*courant] == 0){
    // printf("Indice : %d On lit : %d \n", *courant, entree[*courant]);
    *courant = *courant + 1;
    nb_zero++;
  }
  if (*courant == 64) {
    return 64; //EOB
  } else if (nb_zero >= 16) {
    *courant = *courant - (nb_zero - 16);
    nb_zero = 16; //ZRL
  }
    return nb_zero;
  }

void ZRL(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg){
  // uint8_t zrl = 240;
  uint8_t valeur_symbole = 240;
  uint8_t longeur_huffman = 0;
  uint32_t chemin_huffman = 0;
  chemin_huffman = huffman_table_get_path(jpeg_desc_get_huffman_table(jpeg, AC, Y), valeur_symbole, &longeur_huffman);
  bitstream_write_nbits(bitstream_jpeg, chemin_huffman, longeur_huffman, 0);
  printf("/F0 \n");
}

void EOB(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg){
  // uint8_t eob = 0;
  uint8_t valeur_symbole = 0;
  uint8_t longeur_huffman = 0;
  uint32_t chemin_huffman = 0;
  chemin_huffman = huffman_table_get_path(jpeg_desc_get_huffman_table(jpeg, AC, Y), valeur_symbole, &longeur_huffman);
  bitstream_write_nbits(bitstream_jpeg, chemin_huffman, longeur_huffman, 0);
  printf("/00\n");
}

void balise_std(int nb_zero, int valeur, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg){
  uint32_t m = obtenir_magnetude(valeur);
  uint32_t i = obtenir_indice(valeur, m);
  if (m == 0) {
    perror("On ne lit pas assez de 0 ! \n");
    exit(EXIT_FAILURE);
  }
  uint8_t valeur_symbole = 16*nb_zero + m;
  uint8_t longeur_huffman = 0;
  uint32_t chemin_huffman = huffman_table_get_path(jpeg_desc_get_huffman_table(jpeg, AC, Y), valeur_symbole, &longeur_huffman);
  bitstream_write_nbits(bitstream_jpeg, chemin_huffman, longeur_huffman, 0);
  bitstream_write_nbits(bitstream_jpeg, i, m, 0);
  // printf("nb_zero = %u, valeur = %d, symbole = %s, huffman = %s, sur %d bits, indice = %s \n",nb_zero, valeur, hexme(valeur_symbole), binme_n(chemin_huffman, longeur_huffman), longeur_huffman, binme_n(i,m));
}

void LRE(int16_t *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg){
  int courant = 1;
  while (courant < 64) {
    int nb_zero = compter_zero(entree, &courant);
    // affichage_ind_magn(entree[courant]);
    switch(nb_zero){
      case 16:
        ZRL(bitstream_jpeg, jpeg);
        break;
      case 64:
        EOB(bitstream_jpeg, jpeg);
        break;
      default:
        balise_std(nb_zero, entree[courant], bitstream_jpeg, jpeg);
        courant++;
        break;
    }
  }
}

//Partie adaptation à la structure;

int32_t calcul_DC(int16_t *flux, int premier, int32_t *DC){
    int32_t val_DC = flux[0];
    if (premier != 0){
      val_DC = val_DC - *DC;
    }
    *DC = flux[0];
    return val_DC;
}

void ACDC_me(struct Image_MCU_16 *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg){
  int premier = 0;
  int32_t DC_valeur = 0;
  int32_t DC_precedent = 0;
  uint8_t len_chemin = 0;
  uint32_t code_dc = 0;
  for (uint32_t parcours=0; parcours < entree->hauteur * entree->largeur; parcours++){
      DC_valeur = calcul_DC(entree->MCUs[parcours]->flux, premier, &DC_precedent);
      uint32_t m = obtenir_magnetude(DC_valeur);
      uint32_t i = obtenir_indice(DC_valeur, m);
      code_dc = huffman_table_get_path(jpeg_desc_get_huffman_table(jpeg, DC, Y), m, &len_chemin);
      printf("MCU = %d :      DC = %d, m=%d, encodé = %s, indice = %s \n",parcours, DC_valeur, m, binme_n(code_dc, len_chemin), binme_n(i,m));
      bitstream_write_nbits(bitstream_jpeg, code_dc, len_chemin, 0);
      bitstream_write_nbits(bitstream_jpeg, i, m, 0);
      LRE(entree->MCUs[parcours]->flux, bitstream_jpeg, jpeg);
      if (premier == 0){premier++;}
  }
}
