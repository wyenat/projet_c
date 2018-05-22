//
// Created by antoine on 17/05/116.
//

#ifndef ETU_ZIG_ZAG_H
#define ETU_ZIG_ZAG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "DCT.h"

// fonction principale qui sera appelée par le programme
// prend en entrée une image
// renvoie cette image ou chaque bloc 8x8 (x16 bits) a été zig-zagué
void zig_zag_image(struct Image_MCU_16 *image);

// zig-zag chaque bloc 8x8 du MCU
void zig_zag_MCU(struct MCU_16 *MCU);

// en entrée un vecteur de longueur 64
// en sortie ce vecteur réordonné suivant zig_zag
void zig_zag_8x8(int16_t *vecteur);

void afficher_zig_zag(int16_t *vecteur);

#endif //ETU_ZIG_ZAG_H
