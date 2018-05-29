#include "zig_zag.h"

// Ces structures sont là pour les retrouver plus vite en codant ou en relisant
// Le fonctionnnement de ce script est très semblable à celui de quantification.c

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
// renvoie cette image ou chaque bloc 8x8 (x16 bits) a été zig-zagué
void zig_zag_image(struct Image_MCU_16 *image)
{
    struct MCU_16 *MCU;
    // cette fonction appelle le zig_zag sur chaque MCU de l'image
    for ( uint32_t indice = 0; indice < image->hauteur*image->largeur; indice++) {
        MCU = image->MCUs[indice];
        zig_zag_MCU(MCU);
    }
}


void zig_zag_MCU(struct MCU_16 *MCU)
{
    uint8_t larg = MCU->largeur;
    uint8_t haut = MCU->hauteur;
    uint8_t n_blocs = larg*haut;
    if (MCU->couleur == 1) { // si on est en couleur le nombre de bloc n'est pas trivial
        // Le nombre de blocs différent est toujours largeur * hauteur pour Y, mais pour Cb et Cr il dépend de l'échantillonage.
        // Si l'échantillonage est vertical, il n'y a plus que hauteur/2 blocs, et idem pour un échantillonage horizontal.
        n_blocs += 2*(larg - MCU->echant_h*larg/2)*(haut - MCU->echant_h*haut/2);
    }
    // Il nous faut une table tampon pour faire notre zig_zag
    // Ce n'est pas possible en place
    int16_t *vecteur = malloc(64*sizeof(int16_t));
    // cette fonction appelle le zig_zag sur chaque bloc 8x8 du MCU
    for (uint8_t bloc = 0; bloc < n_blocs; bloc++) {
        for (int indice = 0; indice < 64; indice++) {
            vecteur[indice] = MCU->flux[64*bloc + indice];
        }
        zig_zag_8x8(vecteur);
        for (int indice = 0; indice < 64; indice++) {
            MCU->flux[64*bloc + indice] = vecteur[indice];
        }
    }
    free(vecteur);
}



// en entrée un vecteur de longueur 64
// en sortie ce vecteur réordonné suivant zig_zag
void zig_zag_8x8(int16_t *vecteur)
{
    // Cette méthode est un petit peu bourrine,
    // On a fait un vecteur de correspondance à la main
    int16_t correspondance_zigzag[64] = {  0,  1,  8, 16,  9,  2,  3, 10,
                                          17, 24, 32, 25, 18, 11,  4,  5,
                                          12, 19, 26, 33, 40, 48, 41, 34,
                                          27, 20, 13,  6,  7, 14, 21, 28,
                                          35, 42, 49, 56, 57, 50, 43, 36,
                                          29, 22, 15, 23, 30, 37, 44, 51,
                                          58, 59, 52, 45, 38, 31, 39, 46,
                                          53, 60, 61, 54, 47, 55, 62, 63};
    // vecteur tampon car zig_zag n'est pas faisable en place
    int16_t vecteur_zigzag[64];
    for (int indice = 0; indice < 64; indice++) {
        vecteur_zigzag[indice] = vecteur[correspondance_zigzag[indice]];
    }
    for (int indice = 0; indice < 64; indice++) {
        vecteur[indice] = vecteur_zigzag[indice];
    }
}



void afficher_zig_zag(int16_t *vecteur)
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
