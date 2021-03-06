//
// Created by antoine on 18/05/18.
//

#ifndef ETU_DCT_H
#define ETU_DCT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "decoupage_MCU.h"
#include "recuperer_image.h"


struct MCU_16 {                // information sur un octet (RGB, YCbCr et échantillonnage)
    int couleur;              //0 si en noir et blanc, 1 sinon.
    uint8_t largeur;          // 1 ou 2 (blocs de 8x8)
    uint8_t hauteur;          // 1 ou 2 (blocs de 8x8)
    uint8_t echant_l;         // 1 si on échantillonne en largeur, 0 sinon
    uint8_t echant_h;         // 1 si on échantillonne en hauteur, 0 sinon
    int16_t *flux;
};

struct Image_MCU_16 {
    int couleur;              //0 si en noir et blanc, 1 sinon.
    uint32_t largeur;         // en MCUs
    uint32_t hauteur;         // en MCUs
    struct MCU_16 **MCUs;
};


int16_t * transformation_DCT(uint8_t *bloc_spatial, size_t indice_depart);


struct MCU_16 * transfo_MCU(struct MCU_8 *MCU_entree);

/* Libère la mémoire allouée à l'image MCU_16 */
void Image_destroy(struct Image_MCU_16 *entree);


struct Image_MCU_16 * Image_DCT(struct Image_MCU_8 *Image_entree);


void afficher_DCT(struct MCU_16 *MCU);


void afficher_image_DCT(struct Image_MCU_16 *image);

#endif //ETU_DCT_H
