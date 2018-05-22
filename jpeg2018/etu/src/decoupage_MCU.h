//
// Created by Antonin on 18/05/18
//

#ifndef DECOUPAGE_MCU
#define DECOUPAGE_MCU

#include "recuperer_image.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct MCU_8 {                // information sur un octet (RGB, YCbCr et échantillonnage)
    int couleur;              //0 si en noir et blanc, 1 sinon.
    uint8_t largeur;          // 1 ou 2 (blocs de 8x8)
    uint8_t hauteur;          // 1 ou 2 (blocs de 8x8)
    uint8_t echant_l;         // 1 si on échantillonne en largeur, 0 sinon
    uint8_t echant_h;         // 1 si on échantillonne en hauteur, 0 sinon
    uint8_t *flux;
};

struct Image_MCU_8 {
    int couleur;              //0 si en noir et blanc, 1 sinon.
    uint32_t largeur;         // en MCUs
    uint32_t hauteur;         // en MCUs
    struct MCU_8 **MCUs;
};

// découpage d'une image en MCUs
// largeur et hauteur MCU valent 1 ou 2
// renvoie l'image découpé, c'est à dire avec un pointeur sur des pointeurs sur des MCUs
struct Image_MCU_8 *decoupe(struct Image *image, uint8_t largeur_MCU, uint8_t hauteur_MCU);

void afficher_image_8(struct Image_MCU_8 *image);

void afficher_MCU_8(struct MCU_8 *MCU);

// récupére la partie de l'image concerné
// la stock dans le flux du MCU
struct MCU_8 *creer_MCU(struct Image *image, uint32_t ligne, uint32_t colonne, uint32_t largeur, uint8_t largeur_MCU, uint8_t hauteur_MCU);

// s'il y de la couleur, il faut réaranger en 3 blocs ou plus 8x8 (RGB)
// donc pour du 2x1 : R1 G1 B1  R2 G2 B2
struct MCU_8 *MCU_RGB(struct MCU_8 *MCU);

// Les images n'ont pas forcement une taille multiple de la taille des MTU
// on élargit donc si besoin en copiant la dernière colonne
// puis on agrandit si besoin avec la dernière ligne
struct Image *redimensionner(struct Image *image, uint8_t largeur_MCU, uint8_t hauteur_MCU);

// rajoute la valeur de la dernière colonne de pixels sur la droite pour coller avec la taille des MCUs
char *elargir(struct Image *image, char *stream_tmp, uint32_t new_largeur);

// rajoute la valeur de la dernière ligne de pixels en bas pour coller avec la taille des MCUs
char *agrandir(char *stream_tmp, uint32_t new_hauteur, uint32_t last_hauteur, uint32_t new_largeur, uint8_t taille_pixel);

#endif //DECOUPAGE_MCU
