//
// Created by antoine on 20/05/18.
//

#ifndef ETU_DOWNSAMPLING_H
#define ETU_DOWNSAMPLING_H

#include "decoupage_MCU.h"

/* Applique le downsampling horizontal à un MCU. Retourne un nouveau MCU. */
struct MCU_8 * downsampling_horizontal_2_blocs(struct MCU_8 * MCU_entree);


/* Applique le downsampling vertical à un MCU. Retourne un nouveau MCU.*/
struct MCU_8 * downsampling_vertical_2_blocs(struct MCU_8 * MCU_entree);


/* Applique la downsampling à une image complète */
void Image_downsampling(struct Image_MCU_8 * Image_entree, int compression_horizontale, int compression_verticale);


/* Fonction d'affichage d'un MCU après downsampling. */
void afficher_apres_compression(struct MCU_8 *MCU);


/* Fonction d'affichage des MCUs de l'image après downsampling */
void afficher_image_compressee(struct Image_MCU_8 * Image_entree);


#endif //ETU_DOWNSAMPLING_H

