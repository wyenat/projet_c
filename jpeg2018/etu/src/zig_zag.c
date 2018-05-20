#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "decoupage_MCU.h"
#include "zig_zag.h"


// en entrée un vecteur de longueur 64
// en sortie ce vecteur réordonné suivant zig_zag
int16_t *zig_zag(int16_t *vecteur_initial)
{
    int8_t correspondance_zigzag[64] = {  0,  1,  5,  6, 14, 15, 27, 28,
                                           2,  4,  7, 13, 16, 26, 29, 42,
                                           3,  8, 12, 17, 25, 30, 41, 43,
                                           9, 11, 18, 24, 31, 40, 44, 53,
                                          10, 19, 23, 32, 39, 45, 52, 54,
                                          20, 22, 33, 38, 46, 51, 55, 60,
                                          21, 34, 37, 47, 50, 56, 59, 61,
                                          35, 36, 48, 49, 57, 58, 62, 63};
    int16_t *vecteur_zigzag = malloc(64*sizeof(int16_t));
    for (int indice = 0; indice < 64; indice++) {
        vecteur_zigzag[indice] = vecteur_initial[correspondance_zigzag[indice]];
    }
    return vecteur_zigzag;
}

 void afficher(int8_t *vecteur)
 {
     printf("\n");
     for (int indice = 0; indice < 64; indice++) {
         printf("%d, ", vecteur[indice]);
         if ((indice+1)%8 == 0) {
             printf("\n");
         }
     }
     printf("\n");
 }
 
 void zag_zig(struct Image_MCU_16 *entree){
     for (uint32_t hauteur=0; hauteur < entree->hauteur; hauteur++){
         for (uint32_t largeur=0; largeur < entree->largeur; largeur++){
             struct MCU_16 *courant_mcu = entree->MCUs[8*hauteur+largeur];
             courant_mcu->flux = zig_zag(courant_mcu->flux);
         }
     }
 }

 // // parce qu'il faut bien tester
 // int main(void)
 // {
 //     int8_t vecteur_test[64];
 //     for (int indice = 0; indice < 64; indice++) {
 //         vecteur_test[indice] = (int8_t) (indice % 8);
 //     }
 //     afficher(vecteur_test);
 //     int8_t *vecteur_zigzag = zig_zag(vecteur_test);
 //     afficher(vecteur_zigzag);
 // }
