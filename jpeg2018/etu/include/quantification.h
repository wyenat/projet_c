//
// Created by antoine on 17/05/18.
//

#ifndef ETU_QUANTIFICATION_H
#define ETU_QUANTIFICATION_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/qtables.h"
#include "DCT.h"

// fonction principale qui sera appelée par le programme
// prend en entrée une image
// renvoie cette image ou chaque bloc 8x8 (x16 bits) a été quantifié
void quantifier_image(struct Image_MCU_16 *image);

void quantifier_MCU(struct MCU_16 *MCU);

// prend en entrée la table à quantifier
// et un int ayant une fonction de booléen valant
// 1 si la table est une table Y, 0 pour Cb/Cr
void quantifier_8x8(int16_t *table, int booleen_tableY);

void afficher_quantifier(int16_t *vecteur);

#endif //ETU_QUANTIFICATION_H
