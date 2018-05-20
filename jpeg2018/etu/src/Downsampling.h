//
// Created by antoine on 20/05/18.
//

#ifndef ETU_DOWNSAMPLING_H
#define ETU_DOWNSAMPLING_H

#endif //ETU_DOWNSAMPLING_H

#include "decoupage_MCU.h"


struct MCU_8 * downsampling_horizontal_2_blocs(struct MCU_8 * MCU_entree);


struct MCU_8 * downsampling_vertical_2_blocs(struct MCU_8 * MCU_entree);


void Image_downsampling(struct Image_MCU_8 * Image_entree, int compression_horizontale, int compression_verticale);


void afficher_apres_compression(struct MCU_8 *MCU);


void afficher_image_compressee(struct Image_MCU_8 * Image_entree);