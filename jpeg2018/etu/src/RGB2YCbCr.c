//
// Created by antoine on 16/05/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


float *RGB2YCbCr_couleur(char *bitstream[], size_t length)  {
    // length est égal à 3 fois le nombre de pixel (3 octets pour le RGB)
    assert(length%3 == 0 && "La longueur donnée en argument n'est pas un multiple de 3 ! L'image est bien une image "
                                    "couleur ? Le bitstream est-il bien correct ? La longueur donnée correspond-elle bien "
                                    "à la longueur du bitstream ?");
    float *vect_YCbCr = malloc(sizeof(float) * length);

    for (size_t i = 0; i < length/3; i++) {
        vect_YCbCr[3*i] = (float) (0.299 * (double)strtol(bitstream[3 * i], NULL, 16) + 0.587 * (double)strtol(bitstream[3 * i+1], NULL, 16) +
                                   0.114 * (double)strtol(bitstream[3 * i+2], NULL, 16));
        printf("Valeur ajoutée dans le nouveau vecteur en Y : %f\n", vect_YCbCr[3*i]);
        vect_YCbCr[3*i+1] = (float) (-0.1687 * (double)strtol(bitstream[3 * i], NULL, 16) - 0.3313 * (double)strtol(bitstream[3 * i+1], NULL, 16) +
                                    0.5 * (double)strtol(bitstream[3 * i+2], NULL, 16) + 128);
        vect_YCbCr[3*i+2] = (float) (0.5 * (double)strtol(bitstream[3 * i], NULL, 16) - 0.4187 * (double)strtol(bitstream[3 * i+1], NULL, 16) -
                                    0.0813 * (double)strtol(bitstream[3 * i+2], NULL, 16) + 128);
    }
    return vect_YCbCr;
}


float *RGB2YCbCr_gris(char* bitstream[], size_t length)  {
    printf("Length : %zu\n", length);
    float *vect_YCbCr = malloc(sizeof(float) * length);

    for (size_t i = 0; i < length; i++) {
        vect_YCbCr[i] = (float) strtol(bitstream[i], NULL, 16);
        printf("Valeur ajoutée dans le nouveau vecteur : %f\n", vect_YCbCr[i]);
    }
    return vect_YCbCr;
}


int main(int argc, char *argv[])    {
    char *bitstream_gris[5] = {"ff", "0f", "00", "ff", "00"};
    float *new_vect_gris = RGB2YCbCr_gris(bitstream_gris, (size_t) 5);
    free(new_vect_gris);
    char *bitstream_couleurs[7] = {"ff", "0f", "00", "15", "a3", "4d"};
    float *new_vect_couleurs = RGB2YCbCr_couleur(bitstream_couleurs, (size_t) 6);
    free(new_vect_couleurs);
    return 0;
}