//
// Created by antoine on 16/05/18.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "decoupage_MCU.h"
#include "hex.h"

struct MCU_8 *RGB2YCbCr_couleur(struct MCU_8 *MCU_entree)  {
    //Convertit le MCU en COULEUR de RGB à YCbCr
    uint16_t nombre_pixels = (uint16_t) (64 * MCU_entree->largeur * MCU_entree->hauteur);
    // printf("Nombre de pixels : %d\n", nombre_pixels);
    // la longueur totale est égal à 3 fois le nombre de pixel (3 octets pour le RGB)
    struct MCU_8 *MCU_YCbCr = malloc(sizeof(struct MCU_8));
    MCU_YCbCr->largeur = MCU_entree->largeur;
    MCU_YCbCr->hauteur = MCU_entree->hauteur;
    MCU_YCbCr->couleur = MCU_entree->couleur;
    MCU_YCbCr->echant_h = MCU_entree->echant_h;
    MCU_YCbCr->echant_l = MCU_entree->echant_l;
    MCU_YCbCr->flux = malloc(3 * nombre_pixels * sizeof(uint8_t));

    for (size_t i = 0; i < nombre_pixels; i++) {
        MCU_YCbCr->flux[i] = (uint8_t) (0.299 * MCU_entree->flux[3*i] + 0.587 * MCU_entree->flux[3*i+1] +
                                   0.114 * MCU_entree->flux[3*i+2]);
        MCU_YCbCr->flux[i + nombre_pixels] = (uint8_t) (-0.1687 * MCU_entree->flux[3*i] - 0.3313 * MCU_entree->flux[3*i+1] +
                                    0.5 * MCU_entree->flux[3*i+2] + 128);
        MCU_YCbCr->flux[i + 2*nombre_pixels] = (uint8_t) (0.5 * MCU_entree->flux[3*i] - 0.4187 * MCU_entree->flux[3*i+1] -
                                    0.0813 * MCU_entree->flux[3*i+2] + 128);
    }
    free(MCU_entree->flux);
    free(MCU_entree);
    return MCU_YCbCr;
}


struct MCU_8 *RGB2YCbCr_gris(struct MCU_8 *MCU_entree)  {
    //Convertit le MCU en NIVEAUX DE GRIS de RGB à YCbCr
    uint16_t nombre_pixels = (uint16_t) (64 * MCU_entree->largeur * MCU_entree->hauteur);
    struct MCU_8 *MCU_YCbCr = malloc(sizeof(struct MCU_8));
    MCU_YCbCr->largeur = MCU_entree->largeur;
    MCU_YCbCr->hauteur = MCU_entree->hauteur;
    MCU_YCbCr->couleur = MCU_entree->couleur;
    MCU_YCbCr->echant_h = MCU_entree->echant_h;
    MCU_YCbCr->echant_l = MCU_entree->echant_l;
    MCU_YCbCr->flux = malloc(nombre_pixels * sizeof(uint8_t));

    for (size_t i = 0; i < nombre_pixels; i++) {
        MCU_YCbCr->flux[i] = (uint8_t) MCU_entree->flux[i];
    }
    free(MCU_entree->flux);
    free(MCU_entree);
    return MCU_YCbCr;
}

void Image_RGB2YCbCr(struct Image_MCU_8 *Image_entree)  {
    uint64_t nombre_MCUs = Image_entree->largeur * Image_entree->hauteur;
    if (Image_entree->couleur == 0) {
        for (uint64_t i = 0; i < nombre_MCUs; i++) {
            Image_entree->MCUs[i] = RGB2YCbCr_gris(Image_entree->MCUs[i]);
        }
    }
    else    {
        for (uint64_t i = 0; i < nombre_MCUs; i++) {
            Image_entree->MCUs[i] = RGB2YCbCr_couleur(Image_entree->MCUs[i]);
        }
    }
}


void afficher_YCbCr(struct MCU_8 *MCU)
{
    uint8_t nombre_composantes = 1 + 2*MCU->couleur;
    uint8_t nombre_blocs = MCU->largeur*MCU->hauteur;
    printf("Nombre de blocs dans le MCU : %d\n", nombre_blocs);
    for (int composante = 0; composante < nombre_composantes; composante++) {
        if (composante == 0) {
            printf("Y\t");
        } else if (composante == 1) {
            printf("Cb\t");
        } else {
            printf("Cr\t");
        }
        for (uint32_t indice = 0; indice < 64 * nombre_blocs; indice++) {
            char *hex = hexme(MCU->flux[64 * nombre_blocs * composante + indice]);
            printf("%s\t", hex);
            // free(hex);
            if (nombre_blocs == 4)  {
                if ((indice + 1) % 128 == 0) {
                    printf("\n\n\t");
                }
                else if ((indice + 1) % 16 == 8) {
                    printf("\t");
                }
                else if ((indice + 1) % 16 == 0) {
                    printf("\n\t");
                }
            }
            else if (MCU->largeur == 2) {
                if ((indice + 1) % 16 == 8) {
                    printf("\t");
                }
                else if ((indice + 1) % 16 == 0) {
                    printf("\n\t");
                }
            }
            else if (MCU->hauteur == 2) {
                if ((indice + 1) % 64 == 0) {
                    printf("\n\n\t");
                }
                else if ((indice + 1) % 16 == 0) {
                    printf("\n\t");
                }
            }
            else    {
                if ((indice + 1) % 8 == 0) {
                    printf("\n\t");
                }
            }
        }
        printf("\n");
    }
}


void afficher_image_YCbCr(struct Image_MCU_8 *image)
{
    struct MCU_8 **MCUs = image->MCUs;
    printf("Image MCU de taille %ux%u\n", image->hauteur, image->largeur);
    for (uint32_t indice = 0; indice < image->largeur*image->hauteur; indice++) {
        struct MCU_8 *MCU = MCUs[indice];
        printf("MCU numéro : %u\n", indice);
        afficher_YCbCr(MCU);
    }
}
