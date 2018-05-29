//
// Created by antoine on 17/05/18.
//

#ifndef ETU_RGB2YCBCR_H
#define ETU_RGB2YCBCR_H

#include <stdlib.h>
#include <stdint.h>
#include "decoupage_MCU.h"


/* Convertit un MCU couleur RGB en MCU couleur YCbCr */
struct MCU_8 *RGB2YCbCr_couleur(struct MCU_8 *MCU_entree);


/* Convertit un MCU gris RGB en MCU gris YCbCr */
struct MCU_8 *RGB2YCbCr_gris(struct MCU_8 *MCU_entree);


/* Applique la transformation de RGB à YCbCr à une image complète */
void Image_RGB2YCbCr(struct Image_MCU_8 *Image_entree);


/* Fonction d'affichage d'un MCU après conversion en YCbCr */
void afficher_YCbCr(struct MCU_8 *MCU);


/* Fonction d'affichage de l'image après conversion en YCbCr */
void afficher_image_YCbCr(struct Image_MCU_8 *image);


#endif //ETU_RGB2YCBCR_H

