#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"
#include "DCT.h"
#include "bitstream_perso.h"
#include "jpeg_writer_perso.h"

uint32_t  obtenir_magnetude(int16_t entree);

uint32_t  obtenir_indice(int16_t entree, int8_t  magnetude);

void affichage_ind_magn(int16_t entree);

int compter_zero(int16_t *entree, int *courant);

void ZRL(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

void EOB(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

void balise_std(int nb_zero, int valeur, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

void LRE(int16_t *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose, int layer_y, int nb_compression);

int calcul_DC(int16_t *flux, int premier,int32_t *DC, int couleur, int verbose, int layer_y, int nb_compression);

void ACDC_me(struct Image_MCU_16 *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int verbose);
