//
// Created by antoine on 16/05/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

struct YCbCr    {
    float *Y[];
    float *Cb[];
    float *Cr[];
};


void main(char *bitstream[], bool couleur)  {

    if (couleur) {
        struct YCbCr vect_YCbCr;
        length = sizeof(bitstream) / sizeof(bitstream[0]);

        vect_YCbCr.Y = malloc(length * sizeof(float));
        vect_YCbCr.Cb = malloc(length * sizeof(float));
        vect_YCbCr.Cr = malloc(length * sizeof(float));


        length = sizeof(bitstream) / sizeof(bitstream[0]);
        for (int i = 0; i < length; i++) {
            vect_YCbCr.Y[i] = (float) (0.299 * bistream[3 * i] + 0.587 * bitstream[3 * i + 1] +
                                       0.114 * bitstream[3 * i + 2]);
            vect_YCbCr.Cb[i] = (float) (-0.1687 * bistream[3 * i] - 0.3313 * bitstream[3 * i + 1] +
                                        0.5 * bitstream[3 * i + 2] + 128);
            vect_YCbCr.Cr[i] = (float) (0.5 * bistream[3 * i] - 0.4187 * bitstream[3 * i + 1] -
                                        0.0813 * bitstream[3 * i + 2] + 128);
        }
        free(bitstream);
        bitstream = vect_YCbCr;
    }
    // Si ce n'est pas en couleur, on laisse bitstream tel quel.

}