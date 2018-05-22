#include "quantification.h"
// struct MCU_16 {                // information sur un octet (RGB, YCbCr et échantillonnage)
//     int couleur;              //0 si en noir et blanc, 1 sinon.
//     uint8_t largeur;          // 1 ou 2 (blocs de 8x8)
//     uint8_t hauteur;          // 1 ou 2 (blocs de 8x8)
//     uint8_t echant_l;         // 1 si on échantillonne en largeur, 0 sinon
//     uint8_t echant_h;         // 1 si on échantillonne en hauteur, 0 sinon
//     int16_t *flux;
// };
//
// struct Image_MCU_16 {
//     int couleur;              //0 si en noir et blanc, 1 sinon.
//     uint32_t largeur;         // en MCUs
//     uint32_t hauteur;         // en MCUs
//     struct MCU_16 **MCUs;
// };



// fonction principale qui sera appelée par le programme
// prend en entrée une image
// renvoie cette image ou chaque bloc 8x8 (x16 bits) a été quantifié
void quantifier_image(struct Image_MCU_16 *image)
{
    struct MCU_16 *MCU;
    for ( uint32_t indice = 0; indice < image->hauteur*image->largeur; indice++) {
        MCU = image->MCUs[indice];
        quantifier_MCU(MCU);
    }
}


void quantifier_MCU(struct MCU_16 *MCU)
{
    uint8_t larg = MCU->largeur;
    uint8_t haut = MCU->hauteur;
    uint8_t n_blocs = larg*haut;
    if (MCU->couleur == 1) {
        // Le nombre de blocs différent est toujours largeur * hauteur pour Y, mais pour Cb et Cr il dépend de l'échantillonage.
        // Si l'échantillonage est vertical, il n'y a plus que hauteur/2 blocs, et idem pour un échantillonage horizontal.
        n_blocs += 2*(larg - MCU->echant_h*larg/2)*(haut - MCU->echant_h*haut/2);
    }

    int16_t *table = malloc(64*sizeof(int16_t));
    for (uint8_t bloc = 0; bloc < n_blocs; bloc++) {
        for (int indice = 0; indice < 64; indice++) {
            table[indice] = MCU->flux[64*bloc + indice];
        }
        int booleen_tableY = 1;
        if ((MCU->couleur == 1) && (bloc >= haut*larg)) {
            booleen_tableY = 0;
        }
        quantifier_8x8(table, booleen_tableY);
        for (int indice = 0; indice < 64; indice++) {
            MCU->flux[64*bloc + indice] = table[indice];
        }
    }
    free(table);
}



// prend en entrée la table à quantifier
// et un int ayant une fonction de booléen valant
// 1 si la table est une table Y, 0 pour Cb/Cr
void quantifier_8x8(int16_t *table, int booleen_tableY)
{
    uint8_t *table_quantification = NULL;
    if (booleen_tableY == 1) {
        table_quantification = compressed_Y_table;
    } else {
        table_quantification = compressed_CbCr_table;
    }

    int16_t table_quantifiee[64];
    for (uint8_t indice = 0; indice < 64; indice++) {
        float ratio = table[indice]/table_quantification[indice];
        table_quantifiee[indice] = (int16_t)(ratio+0.5)/1;
    }
    for (uint8_t indice = 0; indice < 64; indice++) {
        table[indice] = table_quantifiee[indice];
    }
}

void afficher_quantifier(int16_t *vecteur)
{
    printf("\n");
    for (int indice = 0; indice < 64; indice++) {
        printf("%02hx\t", vecteur[indice]);
        if ((indice+1)%8 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
