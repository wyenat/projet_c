//
// Created by antoine on 17/05/18.
//

#ifndef ETU_ZIG_ZAG_H
#define ETU_ZIG_ZAG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "DCT.h"

int16_t *zig_zag(int16_t *vecteur_initial);

/* Fait zig_zag sur tous les MCUs */
void zag_zig(struct Image_MCU_16 *entree);

#endif //ETU_ZIG_ZAG_H