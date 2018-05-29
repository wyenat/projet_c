#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"
#include "DCT.h"
#include "bitstream_perso.h"
#include "jpeg_writer_perso.h"

/* Prend un entier et retourne sa magnitude */
uint32_t  obtenir_magnetude(int16_t entree);

/* Prend un entier et sa magnitude associée et renvoie son indice */
uint32_t  obtenir_indice(int16_t entree, int8_t  magnetude);

/* Affiche un entier en format Magnitude + indice */
void affichage_ind_magn(int16_t entree);

/* Compte le nombre de zéro dans le MCU entrée, entre l'indice courant et la prochaine valeur non-nulle
ou la fin du bloc */
int compter_zero(int16_t *entree, int *courant);

/* Ecrit dans le fichier une balise ZRL (correspond à 16 zéros consécutifs ET encore au moins une valeur non-nulle
avant la fin du bloc) */
void ZRL(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

/* Ecrit dans le fichier une balise EOB (correspond à des 0 jusqu'à la fin du bloc) */
void EOB(struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

/* Ecrit dans le fichier une balise de la forme 0xAB, avec
A : le nombre de zéros calculé par compter_zero
B : la magnétude de l'entier
Puis l'indice de l'entier valeur */
void balise_std(int nb_zero, int valeur, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose);

/* Parcourt tout le bloc en décidant laquelle des trois balises écrire à chaque fois */
void LRE(int16_t *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int couleur, int verbose, int layer_y, int nb_compression);

/* Calcul le DC du bloc, et le retourne */
uint32_t calcul_DC(int16_t *flux, int premier,int32_t *DC, int couleur, int layer_y, int nb_compression);

/* Prend en entrée une image et le fichier de sortie, et écrit les informations contenues dans l'image
dans le fichier avec la compression ACDC */
void ACDC_me(struct Image_MCU_16 *entree, struct bitstream *bitstream_jpeg, struct jpeg_desc *jpeg, int verbose);
