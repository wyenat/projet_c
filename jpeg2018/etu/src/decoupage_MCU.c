#include "decoupage_MCU.h"

// struct Image {
//    int couleur;               //0 si en noir et blanc, 1 sinon.
//    uint32_t largeur;
//    uint32_t hauteur;
//    uint32_t valeur;
//    char *stream;
//    size_t taille_stream;
// };

// struct MCU_8 {                // information sur un octet (RGB, YCbCr et échantillonnage)
//     int couleur;              //0 si en noir et blanc, 1 sinon.
//     uint8_t largeur;          // 1 ou 2 (blocs de 8x8)
//     uint8_t hauteur;          // 1 ou 2 (blocs de 8x8)
//     uint8_t echant_l;         // 1 si on échantillonne en largeur, 0 sinon
//     uint8_t echant_h;         // 1 si on échantillonne en hauteur, 0 sinon
//     uint8_t *flux;
// };

// struct Image_MCU_8 {
//     int couleur;              //0 si en noir et blanc, 1 sinon.
//     uint32_t largeur;         // en MCUs
//     uint32_t hauteur;         // en MCUs
//     struct MCU_8 **MCUs;
// };


// découpage d'une image en MCUs
// largeur et hauteur MCU valent 1 ou 2
struct Image_MCU_8 *decoupe(struct Image *image, uint8_t largeur_MCU, uint8_t hauteur_MCU)
{
    // struct Image *image = redimensionner(ancienne_image, largeur_MCU, hauteur_MCU);
    uint32_t largeur = (image->largeur)/(8*largeur_MCU);          //en MCUs
    uint32_t hauteur = (image->hauteur)/(8*hauteur_MCU);
    struct MCU_8 **MCUs = malloc(hauteur*largeur*sizeof(struct MCU *));
    for (uint32_t ligne = 0; ligne < hauteur; ligne++) {
        for (uint32_t colonne = 0; colonne < largeur; colonne++) {
            struct MCU_8 *MCU = creer_MCU(image, ligne, colonne, largeur, largeur_MCU, hauteur_MCU);
            MCU = MCU_RGB(MCU);
            MCUs[largeur*ligne + colonne] = MCU;
        }
    }
    struct Image_MCU_8 *image_MCU = malloc(sizeof(struct Image_MCU_8));
    image_MCU->couleur = image->couleur;
    image_MCU->largeur = largeur;
    image_MCU->hauteur = hauteur;
    image_MCU->MCUs = MCUs;
    return image_MCU;
}


void afficher_image_8(struct Image_MCU_8 *image)
{
    struct MCU_8 **MCUs = image->MCUs;
    printf("Image MCU de taille %ux%u\n", image->hauteur, image->largeur);
    for (uint32_t indice = 0; indice < image->largeur*image->hauteur; indice++) {
        struct MCU_8 *MCU = MCUs[indice];
        printf("MCU numéro : %u\n", indice);
        afficher_MCU_8(MCU);
    }
}


void afficher_MCU_8(struct MCU_8 *MCU)
{
    uint8_t taille_pixel = 1 + 2*MCU->couleur;         // 1 si noir et blanc, 3 si couleur, nombre d'octets par pixel
    for (uint8_t bloc = 0; bloc < MCU->largeur*MCU->hauteur; bloc++) {
        for (uint8_t couleur = 0; couleur < taille_pixel; couleur++) {
            if (MCU->couleur == 1) {
                if (couleur == 0) {
                    printf("R\t");
                } else if (couleur == 1) {
                    printf("G\t");
                } else {
                    printf("B\t");
                }
            } else {
                printf("\t");
            }
            for (uint32_t indice = 0; indice < 64; indice++) {
                printf("%u\t", MCU->flux[64*taille_pixel*bloc + 64*couleur + indice]);
                if ((indice+1)%8 == 0) {
                    printf("\n\t");
                }
            }
            printf("\n");
        }
    }
}


struct MCU_8 *creer_MCU(struct Image *image, uint32_t ligne, uint32_t colonne, uint32_t largeur, uint8_t largeur_MCU, uint8_t hauteur_MCU)
{
    uint8_t taille_pixel = 1 + 2*image->couleur;         // 1 si noir et blanc, 3 si couleur, nombre d'octets par pixel
    uint8_t *flux = malloc(64*taille_pixel*largeur_MCU*hauteur_MCU*sizeof(uint8_t));
    for (uint32_t ordonnee = 0; ordonnee < 8*hauteur_MCU; ordonnee++) {
        for (uint32_t abscisse = 0; abscisse < 8*taille_pixel*largeur_MCU; abscisse++) {
            flux[8*taille_pixel*largeur_MCU*ordonnee + abscisse] = image->stream[(8*taille_pixel*largeur_MCU*largeur)*(8*hauteur_MCU*ligne + ordonnee) + (8*taille_pixel*largeur_MCU*colonne + abscisse)];
        }
    }
    struct MCU_8 *MCU = malloc(sizeof(struct MCU_8));
    MCU->couleur = image->couleur;
    MCU->largeur = largeur_MCU;
    MCU->hauteur = hauteur_MCU;
    MCU->flux = flux;
    return MCU;
}


// s'il y de la couleur, il faut réaranger en 3 blocs ou plus 8x8 (RGB)
// donc pour du 2x1 : R1 G1 B1  R2 G2 B2
struct MCU_8 *MCU_RGB(struct MCU_8 *MCU)
{
    if (MCU->couleur == 0) {
        return MCU;
    }
    uint8_t haut = MCU->hauteur, larg = MCU->largeur;
    uint8_t RGB[3*64*haut*larg];
    for (uint8_t h = 0; h < haut; h ++) {
        for (uint8_t l = 0; l < haut; l ++) {
            for (uint8_t ligne = 0; ligne < 8; ligne++) {
                for (uint8_t colonne = 0; colonne < 8; colonne++) {
                    RGB[64*(3*(2*h +l)) + 8*ligne + colonne] = MCU->flux[larg*8*3*(8*h+ligne)+ 8*l+colonne];
                    RGB[64*(3*(2*h +l)+1) + 8*ligne + colonne] = MCU->flux[larg*8*3*(8*h+ligne)+ 8*l+colonne+1];
                    RGB[64*(3*(2*h +l)+2) + 8*ligne + colonne] = MCU->flux[larg*8*3*(8*h+ligne)+ 8*l+colonne+2];
                }
            }
        }
    }
    return MCU;
}



// // Les images n'ont pas forcement une taille multiple de la taille des MTU
// struct Image *redimensionner(struct Image *image, uint8_t largeur_MCU, uint8_t hauteur_MCU)
// {
//     int elargir_droite = 0, elargir_bas = 0;        // 1 s'il faut élargir
//     uint32_t new_largeur = image->largeur, new_hauteur = image->hauteur;
//     uint8_t taille_pixel = 1 + 2*image->couleur;         // 1 si noir et blanc, 3 si couleur, nombre d'octets par pixel
//     if (image->largeur % (8*largeur_MCU) != 0) {
//         elargir_droite = 1;
//         printf("Il faut élargir\n");
//         new_largeur = image->largeur + (8*largeur_MCU - (image->largeur %(8*largeur_MCU)));
//     }
//     if (image->hauteur % (8*hauteur_MCU) != 0) {
//         elargir_bas = 1;
//         printf("Il faut agrandir\n");
//         new_hauteur = image->hauteur + (8*hauteur_MCU - (image->hauteur %(8*hauteur_MCU)));
//     }
//     uint8_t *stream_tmp = malloc(new_hauteur*new_largeur*taille_pixel*sizeof(uint8_t));
//     if (elargir_droite == 1) {
//         elargir(image, stream_tmp, new_largeur);
//     }
//     if (elargir_bas == 1) {
//         agrandir(image, stream_tmp, new_hauteur);
//     }
//     return image; //TODO faut retourner un autre truc !
// }
//
// // rajoute la valeur de la dernière colonne de pixels sur la droite pour coller avec la taille des MCUs
// uint8_t *elargir(struct Image *image, uint8_t *stream_tmp, uint32_t new_largeur)
// {
//     //TODO
// }
//
// // rajoute la valeur de la dernière ligne de pixels en bas pour coller avec la taille des MCUs
// uint8_t *agrandir(struct Image *image, uint8_t *stream_tmp, uint32_t new_hauteur)
// {
//     //TODO
// }


// int main(void)
// {
//     struct Image *pic = initialisation("../images/zig-zag.ppm");
//     afficher_pic(pic);
//     struct Image_MCU_8 *image = decoupe(pic, 1, 1);
//     afficher_image_8(image);
//     return EXIT_SUCCESS;
// }
