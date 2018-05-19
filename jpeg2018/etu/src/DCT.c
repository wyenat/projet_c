//
// Created by antoine on 17/05/18.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "decoupage_MCU.h"
#include "recuperer_image.h"
#include "DCT.h"


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


//struct MCU_8 {                // information sur un octet (RGB, YCbCr et échantillonnage)
//    int couleur;              //0 si en noir et blanc, 1 sinon.
//    uint8_t largeur;          // 1 ou 2 (blocs de 8x8)
//    uint8_t hauteur;          // 1 ou 2 (blocs de 8x8)
//    uint8_t echant_l;         // 1 si on échantillonne en largeur, 0 sinon
//    uint8_t echant_h;         // 1 si on échantillonne en hauteur, 0 sinon
//    uint8_t *flux;
//};


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
    free(MCU_entree);
    return new_MCU;

}


int main(void)  {
    uint8_t bloc_spatial[64] = {  0,  1,  5,  6, 14, 15, 27, 28,
                      2,  4,  7, 13, 16, 26, 29, 42,
                      3,  8, 12, 17, 25, 30, 41, 43,
                      9, 11, 18, 24, 31, 40, 44, 53,
                      10, 19, 23, 32, 39, 45, 52, 54,
                      20, 22, 33, 38, 46, 51, 55, 60,
                      21, 34, 37, 47, 50, 56, 59, 61,
                      35, 36, 48, 49, 57, 58, 62, 63};
    struct MCU_8 *MCU_test = malloc(sizeof(struct MCU_8));
    MCU_test->couleur = 0;
    MCU_test->largeur = 1;
    MCU_test->hauteur = 1;
    MCU_test->echant_l = 0;
    MCU_test->echant_h = 0;
    MCU_test->flux = bloc_spatial;
    struct MCU_16 *new_MCU = transfo_MCU(MCU_test);
    for (int i = 0; i < 64; ++i) {
        printf("%hd\n", new_MCU->flux[i]);
    }
    free(new_MCU);
    return 1;
}
