//
// Created by antoine on 17/05/18.
//

#ifndef ETU_RGB2YCBCR_H
#define ETU_RGB2YCBCR_H

#endif //ETU_RGB2YCBCR_H

#include <stdlib.h>
#include <stdint.h>
#include "decoupage_MCU.h"



struct MCU_8 *RGB2YCbCr_couleur(struct MCU_8 *MCU_entree);


struct MCU_8 *RGB2YCbCr_gris(struct MCU_8 *MCU_entree);


void Image_RGB2YCbCr(struct Image_MCU_8 *Image_entree);
