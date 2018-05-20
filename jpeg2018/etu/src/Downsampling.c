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
    int nombre_blocs = MCU_entree->hauteur * MCU_entree->largeur;
    uint8_t *flux_sortie = malloc(64 * sizeof(int16_t) * (nombre_blocs * (2 - MCU_entree->echant_h /(uint8_t)2 )));
    // Si il n'y avait pas de compression verticale, on a besoin de 2 * nombre_blocs
    // Y reste de taille largeur * hauteur, Cb et Cr seront de taille largeur/2 * hauteur (et largeur/2 = 1).
    // Donc en tout, le nouveau flux est de taille largeur * hauteur + 2*(largeur/2 * hauteur) = 2*(largeur * hauteur)

    // Si il y avait déjà de la compression verticale Cb et Cr occupent deux fois moins de place. donc on retire nombre_blocs / 2.

    for (int i = 0; i < nombre_blocs * 64; i++) {
        // On copie dans un premier temps Y
        flux_sortie[i] = MCU_entree->flux[i];
    }
    for (int j= 0; j< 64*nombre_blocs * (1 - MCU_entree->echant_h / (uint8_t) 2); j++)   {
        flux_sortie[nombre_blocs * 64 + j] = (uint8_t) (((uint16_t)MCU_entree->flux[(nombre_blocs * 64) + 2 * j] + (uint16_t) MCU_entree->flux[(nombre_blocs * 64) + 2 * j + 1]) / (uint16_t) 2);;
    }
    struct MCU_8 *MCU_sortie = malloc(sizeof(struct MCU_8));
    MCU_sortie->hauteur = MCU_entree->hauteur;
    MCU_sortie->largeur = MCU_entree->largeur/(uint8_t)2;    // Sera en pratique toujours égal à 1.
    MCU_sortie->couleur = MCU_entree->couleur;          // Idem, en pratique toujours égal à 1, vu qu'on ne compresse que les images couleurs.
    MCU_sortie->echant_l = 1;
    MCU_sortie->echant_h = MCU_entree->echant_h;
    MCU_sortie->flux = flux_sortie;

    free(MCU_entree);
    return MCU_sortie;
}


struct MCU_8 * downsampling_vertical_2_blocs(struct MCU_8 * MCU_entree)   {
    // Compresse un MCU ?x2 en un MCU ?x1. ATTENTION !! Ne marche que sur des MCU de hauteur 2 !

    int nombre_blocs = MCU_entree->hauteur * MCU_entree->largeur * (1 + MCU_entree->echant_l);
    uint8_t *flux_sortie = malloc(64 * sizeof(int16_t) * (nombre_blocs * (2 - MCU_entree->echant_l /(uint8_t)2 )));
    // Si il n'y avait pas de compression horizontale, on a besoin de 2 * nombre_blocs
    // Y reste de taille largeur * hauteur, Cb et Cr seront de taille largeur * hauteur/2 (et hauteur/2 = 1).
    // Donc en tout, le nouveau flux est de taille largeur * hauteur + 2*(largeur * hateur/2) = 2*(largeur * hauteur)

    // Si il y avait déjà de la compression horizontale Cb et Cr occupent deux fois moins de place. donc on retire nombre_blocs / 2.

    for (int i = 0; i < nombre_blocs * 64; i++) {
        // On copie dans un premier temps Y
        flux_sortie[i] = MCU_entree->flux[i];
    }
    for (int ligne=0; ligne < 16; ligne++)   {
        for (int i=0; i < MCU_entree->largeur * 8; i++) {
            flux_sortie[(nombre_blocs * 64) + (ligne * MCU_entree->largeur * 8) + i] = (uint8_t) (((uint16_t)(MCU_entree->flux[(nombre_blocs * 64) + (MCU_entree->largeur * 8 * 2*ligne) + i]
                    + (uint16_t)MCU_entree->flux[(nombre_blocs * 64) + (MCU_entree->largeur * 8 * (2*ligne + 1)) + i])) / (uint16_t) 2);
        }
    }
    struct MCU_8 *MCU_sortie = malloc(sizeof(struct MCU_8));
    MCU_sortie->hauteur = MCU_entree->hauteur/(uint8_t)2;    // Sera en pratique toujours égal à 1.
    MCU_sortie->largeur = MCU_entree->largeur;
    MCU_sortie->couleur = MCU_entree->couleur;          // Idem, en pratique toujours égal à 1, vu qu'on ne compresse que les images couleurs.
    MCU_sortie->echant_l = MCU_entree->echant_l;
    MCU_sortie->echant_h = 1;
    MCU_sortie->flux = flux_sortie;

    free(MCU_entree);
    return MCU_sortie;
}



void Image_downsampling(struct Image_MCU_8 * Image_entree, int compression_horizontale, int compression_verticale)    {
    uint64_t nombre_MCUs = Image_entree->largeur * Image_entree->hauteur;
    for (uint64_t i=0; i < nombre_MCUs; i++)    {
        if (compression_horizontale == 1)    {
            assert((Image_entree->MCUs[i]->largeur == 2) && "Il faut que le MCU soit de largeur 2 si on veut compresser horizontalement !\n");
            Image_entree->MCUs[i] = downsampling_horizontal_2_blocs(Image_entree->MCUs[i]);
        }
        if (compression_verticale == 1)    {
            assert((Image_entree->MCUs[i]->hauteur == 2) && "Il faut que le MCU soit de hauteur 2 si on veut compresser verticalement !\n");
            Image_entree->MCUs[i] = downsampling_vertical_2_blocs(Image_entree->MCUs[i]);
            if (compression_horizontale == 1)   {
                // Il faut rétablir la hauteur du MCU pour l'affichage.
                Image_entree->MCUs[i]->largeur = Image_entree->MCUs[i]->largeur * (uint8_t)2;
            }
            Image_entree->MCUs[i]->hauteur = Image_entree->MCUs[i]->hauteur * (uint8_t)2;
        }
        else    {
            if (compression_horizontale == 1)   {
                Image_entree->MCUs[i]->largeur = Image_entree->MCUs[i]->largeur * (uint8_t)2;
            }
        }
    }
}


void afficher_apres_compression(struct MCU_8 *MCU)
{
    uint8_t nombre_composantes = 1 + 2*MCU->couleur;         // 1 si noir et blanc, 3 si couleur, nombre d'octets par pixel
    uint8_t nombre_blocs = MCU->largeur*MCU->hauteur;
    printf("Nombre de blocs dans le MCU : %d\n", nombre_blocs);
    for (int composante = 0; composante < nombre_composantes; composante++) {
        if (composante == 0) {
            printf("Y\t");
            for (uint32_t indice = 0; indice < 64 * nombre_blocs; indice++) {
                printf("%u\t", MCU->flux[indice]);
                if ((indice + 1) % (64 * MCU->largeur * MCU->hauteur) == 0)   {
                    printf("\n\n");
                }
                else if ((indice + 1) % (64 * MCU->largeur) == 0)   {
                    printf("\n\n\t");
                }
                else if((indice + 1) % 16 == 8) {
                    printf("\t");
                }
                else if((indice + 1) % 16 == 0) {
                    printf("\n\t");
                }
            }
            printf("\n");
        }
        else    {
            if (composante == 1)    {
                printf("Cb\t");
            } else {
                printf("Cr\t");
            }

            if (MCU->echant_h && MCU->echant_l) {
                printf("H et L\n\t");
                for (uint32_t indice = 0; indice < 64; indice++)    {
                    printf("%u\t", MCU->flux[64 * nombre_blocs + 64 * (composante - 1) + indice]);
                    if ((indice + 1) % 64 == 0)   {
                        printf("\n\n");
                    }
                    else if((indice + 1) % 8 == 0) {
                        printf("\n\t");
                    }
                }
            }
            else if (MCU->echant_h) {
                printf(("Juste H\n\t"));
                for (uint32_t indice = 0; indice < MCU->largeur * 64; indice++)    {
                    printf("%u\t", MCU->flux[64 * nombre_blocs + MCU->largeur * 64 * (composante - 1) + indice]);
                    if ((indice + 1) % (64*MCU->largeur) == 0)  {
                        printf("\n\n");
                    }
                    else if ((indice + 1) % 16 == 8) {
                        printf("\t");
                    }
                    else if((indice + 1) % 16 == 0) {
                        printf("\n\t");
                    }
                }
            }
            else if (MCU->echant_l) {
                printf("Juste L\n\t");
                for (uint32_t indice = 0; indice < MCU->hauteur * 64; indice++)    {
                    printf("%u\t", MCU->flux[64 * nombre_blocs + MCU->hauteur * 64 * (composante - 1) + indice]);
                    if ((indice + 1) % 64 == 0) {
                        printf("\n\n\t");
                    }
                    else if((indice + 1) % 8 == 0) {
                        printf("\n\t");
                    }
                }
            }
        }


        }
}


void afficher_image_compressee(struct Image_MCU_8 * Image_entree) {
    struct MCU_8 **MCUs = Image_entree->MCUs;
    printf("Image MCU de taille %ux%u\n", Image_entree->hauteur, Image_entree->largeur);
    for (uint32_t indice = 0; indice < Image_entree->largeur*Image_entree->hauteur; indice++) {
        struct MCU_8 *MCU = MCUs[indice];
        printf("MCU numéro : %u\n", indice);
        afficher_apres_compression(MCU);
    }
}