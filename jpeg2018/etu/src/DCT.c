//
// Created by antoine on 17/05/18.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "decoupage_MCU.h"
#include "recuperer_image.h"
#include "DCT.h"
#include "hex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float C(int i)   {
    if (i == 0) {
        return (float) ((float)1 / sqrt(2));
    }
    else{
        return (float)1;
    }
}


int16_t * transformation_DCT(uint8_t *bloc_spatial, size_t indice_depart)  {
    // Transforme un bloc de 8x8 pixels en blocs de 8x8 fréquences

    int16_t * bloc_frequentiel = malloc(sizeof(int16_t) * 64);
    for (int i = 0; i<8; i++)   {
        for (int j = 0; j < 8 ; j++) {
            int somme = 0;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    somme += (bloc_spatial[indice_depart + 8 * x + y] - 128) * cosf((float) ((2 * x + 1) * M_PI * i) / (2 * 8)) *
                             cosf((float) ((2 * y + 1) * M_PI * j) / (2 * 8));
                }

            }
            bloc_frequentiel[8 * i + j] = (int16_t) (C(i) * C(j) * (float) 2 / 8 * somme);
        }
    }
    return bloc_frequentiel;
}


struct MCU_16 * transfo_MCU(struct MCU_8 *MCU_entree) {
    // Récupère le MCU d'entrée et crée un MCU de fréquences.

    uint8_t largeur = MCU_entree->largeur;
    uint8_t hauteur = MCU_entree->hauteur;
    // Le nombre de blocs différent est toujours largeur * hauteur pour Y, mais pour Cb et Cr il dépend de l'échantillonage.
    // Si l'échantillonage est vertical, il n'y a plus que hauteur/2 blocs, et idem pour un échantillonage horizontal.
    int nombre_blocs = (largeur * hauteur + 2 * (largeur - MCU_entree->echant_h * largeur / 2) * (hauteur - MCU_entree->echant_h * hauteur / 2));
    // nombre_blocs fournit le nombre de blocs Y + Cb + Cr dans le MCU
    struct MCU_16 *new_MCU = malloc(sizeof(struct MCU_16));
    new_MCU->couleur = MCU_entree->couleur;
    new_MCU->largeur = MCU_entree->largeur;
    new_MCU->hauteur = MCU_entree->hauteur;
    new_MCU->echant_l = MCU_entree->echant_l;
    new_MCU->echant_h = MCU_entree->echant_h;
    new_MCU->flux = malloc(nombre_blocs * 64 * sizeof(int16_t));
    for (size_t indice_depart = 0; indice_depart < 64 * (size_t) nombre_blocs; indice_depart += 64)  {
        int16_t *new_flux = transformation_DCT(MCU_entree->flux, (size_t) indice_depart);
        memcpy(new_MCU->flux + indice_depart, new_flux, sizeof(int16_t)*64);
        free(new_flux);
    }
    free(MCU_entree->flux);
    free(MCU_entree);
    return new_MCU;

}

void Image_destroy(struct Image_MCU_16 *entree){
    for (uint64_t i = 0; i < entree->largeur * entree->hauteur; i++) {
        free(entree->MCUs[i]->flux);
        free(entree->MCUs[i]);
    }
    free(entree->MCUs);
    free(entree);
}


struct Image_MCU_16 * Image_DCT(struct Image_MCU_8 *Image_entree)   {
    // Convertit une image composée de MCU_8 en version YCbCR en des MCU_16 en version DCT.

    uint64_t nombre_MCUs = Image_entree->largeur * Image_entree->hauteur;
    struct Image_MCU_16 *new_image = malloc(sizeof(struct Image_MCU_16));
    new_image->couleur = Image_entree->couleur;
    new_image->largeur = Image_entree->largeur;
    new_image->hauteur = Image_entree->hauteur;
    new_image->MCUs = malloc(nombre_MCUs * sizeof(struct MCU_16));
    for (uint64_t i = 0; i < nombre_MCUs; i++) {
        new_image->MCUs[i] = transfo_MCU(Image_entree->MCUs[i]);
    }
    free(Image_entree->MCUs);
    free(Image_entree);
    return new_image;
}


void afficher_DCT(struct MCU_16 *MCU)
{
    uint8_t nombre_composantes = 1 + 2*MCU->couleur;         // 1 si noir et blanc, 3 si couleur, nombre d'octets par pixel
    uint8_t nombre_blocs = MCU->largeur*MCU->hauteur;
    for (int composante = 0; composante < nombre_composantes; composante++) {
        if (composante == 0) {
            printf("Y\t");
        } else if (composante == 1) {
            printf("Cb\t");
        } else {
            printf("Cr\t");
        }
        for (uint32_t indice = 0; indice < 64 * nombre_blocs; indice++) {
            printf("%s\t", hexme(MCU->flux[64 * nombre_blocs * composante + indice]));
            if ((indice + 1) % 8 == 0) {
                printf("\n\t");
            }
        }
        printf("\n");
    }
}


void afficher_image_DCT(struct Image_MCU_16 *image)
{
    struct MCU_16 **MCUs = image->MCUs;
    printf("Image MCU de taille %ux%u\n", image->hauteur, image->largeur);
    for (uint32_t indice = 0; indice < image->largeur*image->hauteur; indice++) {
        struct MCU_16 *MCU = MCUs[indice];
        printf("MCU numéro : %u\n", indice);
        afficher_DCT(MCU);
    }
}
