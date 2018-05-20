//
// Created by antoine on 20/05/18.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "decoupage_MCU.h"

struct MCU_8 * downsampling_horizontal_2_blocs(struct MCU_8 * MCU_entree)   {
    // Compresse un MCU 2x? en un MCU 1x?. ATTENTION !! Ne marche que sur des MCU de largeur 2 !
    uint8_t *flux_sortie = malloc(MCU_entree->hauteur * 64 * sizeof(int16_t));
    for (int8_t bloc_vertical = 0; bloc_vertical<MCU_entree->hauteur; bloc_vertical++) {
        for (int i = 0; i < 64; i++) {
            flux_sortie[64*bloc_vertical + i] = (MCU_entree->flux[2 * (64*bloc_vertical + i)] + MCU_entree->flux[2 * (64*bloc_vertical + i) + 1]) / (uint8_t) 2;
        }
    }
    struct MCU_8 *MCU_sortie = malloc(sizeof(struct MCU_8));
    MCU_sortie->hauteur = MCU_entree->hauteur;
    MCU_sortie->largeur = MCU_sortie->largeur / (uint8_t) 2;    // Sera en pratique toujours égal à 1.
    MCU_sortie->couleur = MCU_entree->couleur;          // Idem, en pratique toujours égal à 1, vu qu'on ne compresse que les images couleurs.
    MCU_sortie->echant_l = 1;
    MCU_sortie->echant_h = MCU_entree->echant_h;
    MCU_sortie->flux = flux_sortie;

    free(MCU_entree);
    return MCU_sortie;
}


struct MCU_8 * downsampling_vertical_2_blocs(struct MCU_8 * MCU_entree)   {
    // Compresse un MCU ?x2 en un MCU ?x1. ATTENTION !! Ne marche que sur des MCU de hauteur 2 !
    uint8_t *flux_sortie = malloc(MCU_entree->largeur * 64 * sizeof(int16_t));
    for (int ligne=0; ligne < 8; ligne++)   {
        for (int i=0; i < MCU_entree->largeur * 8; i++) {
            flux_sortie[(ligne * MCU_entree->largeur * 8) + i] = (MCU_entree->flux[(MCU_entree->largeur * 8 * 2*ligne) + i]
                    + MCU_entree->flux[(MCU_entree->largeur * 8 * (2*ligne + 1)) + i]) / (uint8_t) 2;
        }
    }
    struct MCU_8 *MCU_sortie = malloc(sizeof(struct MCU_8));
    MCU_sortie->hauteur = MCU_entree->hauteur / (uint8_t) 2;    // Sera en pratique toujours égal à 1.
    MCU_sortie->largeur = MCU_sortie->largeur;
    MCU_sortie->couleur = MCU_entree->couleur;          // Idem, en pratique toujours égal à 1, vu qu'on ne compresse que les images couleurs.
    MCU_sortie->echant_l = 1;
    MCU_sortie->echant_h = MCU_entree->echant_h;
    MCU_sortie->flux = flux_sortie;

    free(MCU_entree);
    return MCU_sortie;
}



void Image_downsampling(struct Image_MCU_8 * Image_entree, int compression_horizontale, int compression_verticale)    {
    uint64_t nombre_MCUs = Image_entree->largeur * Image_entree->hauteur;
    for (uint64_t i=0; i < nombre_MCUs; i++)    {
        if (compression_horizontale)    {
            assert((Image_entree->MCUs[i]->largeur == 2) && "Il faut que le MCU soit de largeur 2 si on veut compresser horizontalement !\n");
            Image_entree->MCUs[i] = downsampling_horizontal_2_blocs(Image_entree->MCUs[i]);
        }
        if (compression_verticale)    {
            assert((Image_entree->MCUs[i]->hauteur == 2) && "Il faut que le MCU soit de hauteur 2 si on veut compresser verticalement !\n");
            Image_entree->MCUs[i] = downsampling_vertical_2_blocs(Image_entree->MCUs[i]);
        }
    }
}