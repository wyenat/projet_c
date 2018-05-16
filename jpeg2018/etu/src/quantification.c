#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/qtables.h"

// prend en entrée la table à quantifier
// et un int ayant une fonction de booléen valant
// 1 si la table est une table Y, 0 pour Cb/Cr
int16_t *quantifier(int16_t *table, int booleen_tableY)
{
    uint8_t *table_quantification = NULL;
    if (booleen_tableY == 1) {
        table_quantification = compressed_Y_table;
    } else {
        table_quantification = compressed_CbCr_table;
    }

    int16_t *table_quantifiee = malloc(64*sizeof(int16_t));;
    for (int indice = 0; indice < 64; indice++) {
        float ratio = table[indice]/table_quantification[indice];
        table_quantifiee[indice] = floor(ratio+0.5);
    }
    return table_quantifiee;
}

void afficher(int16_t *vecteur)
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

// parce qu'il faut bien tester
int main(void)
{
    int16_t vecteur_test[64];
    for (int indice = 0; indice < 64; indice++) {
        vecteur_test[indice] = pow(indice, 2);
    }
    afficher(vecteur_test);
    int16_t *vecteur_quantifie = quantifier(vecteur_test, 1);
    afficher(vecteur_quantifie);
    return 0;
}
