//
// Created by antoine on 20/05/18.
//

#ifndef ETU_DOWNSAMPLING_H
#define ETU_DOWNSAMPLING_H

#endif //ETU_DOWNSAMPLING_H

#include "DCT.h"


struct MCU_16 * downsampling_horizontal_2_blocs(struct MCU_16 * MCU_entree);


struct MCU_16 * downsampling_vertical_2_blocs(struct MCU_16 * MCU_entree);


void Image_downsampling(struct Image_MCU_16 * Image_entree, int compression_horizontale, int compression_verticale);