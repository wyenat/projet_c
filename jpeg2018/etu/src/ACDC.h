#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"
#include "DCT.h"

uint8_t  obtenir_magnetude(int16_t entree);

uint8_t  obtenir_indice(int16_t entree, int8_t  magnetude);

void affichage_ind_magn(int16_t entree);

int compter_zero(int16_t *entree, int *courant);

void ZRL();

void EOB();

void balise_std(int nb_zero, int valeur);

void LRE(int16_t *entree);

int calcul_DC(int16_t *flux);

void ACDC_me(struct Image_MCU_16 *entree);
