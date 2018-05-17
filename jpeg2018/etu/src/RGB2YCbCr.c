//
// Created by antoine on 16/05/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>




float *RGB2YCbCr_couleur(char *bitstream)  {

    float *vect_YCbCr = malloc(sizeof(bitstream));

    size_t length = sizeof(bitstream) / sizeof(bitstream[0]);


    for (size_t i = 0; i < length; i++) {
        vect_YCbCr[3*i] = (float) (0.299 * (double)strtol(&bitstream[3 * i], NULL, 16) + 0.587 * (double)strtol(&bitstream[3 * i+1], NULL, 16) +
                                   0.114 * (double)strtol(&bitstream[3 * i+2], NULL, 16));
        vect_YCbCr[3*i+1] = (float) (-0.1687 * (double)strtol(&bitstream[3 * i], NULL, 16) - 0.3313 * (double)strtol(&bitstream[3 * i+1], NULL, 16) +
                                    0.5 * (double)strtol(&bitstream[3 * i+2], NULL, 16) + 128);
        vect_YCbCr[3*i+2] = (float) (0.5 * (double)strtol(&bitstream[3 * i], NULL, 16) - 0.4187 * (double)strtol(&bitstream[3 * i+1], NULL, 16) -
                                    0.0813 * (double)strtol(&bitstream[3 * i+2], NULL, 16) + 128);
    }
    free(bitstream);
    return vect_YCbCr;
    // Si ce n'est pas en couleur, on laisse bitstream tel quel.
}


float *RGB2YCbCr_gris(char *bitstream)  {

    float *vect_YCbCr = malloc(sizeof(bitstream));

    size_t length = sizeof(bitstream) / sizeof(bitstream[0]);


    for (size_t i = 0; i < length; i++) {
        vect_YCbCr[i] = (float) bitstream[i];
    }
    free(bitstream);
    return vect_YCbCr;
}


int main(int argc, char *argv[])    {
    char bitstream[5] = {'ff', '0f', '00', 'ff', '00'};
    if (couleur)    {
        RGB2YCbCr_couleur(bitstream);
    }
    else    {
        RGB2YCbCr_gris(bitstream)
    }

    return 0;
}