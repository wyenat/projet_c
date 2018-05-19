#ifndef JPEG_W_H
#define JPEG_W_H

#include <stdio.h>

/* Ouvre un fichier jpeg dans le répertoire courant du nom de nom_du_fichier
Met /FF/D8 (SOI) en binaire écrit dedans 
Retourne un pointeur vers l'image créée */
FILE *creer_fichier_jpeg(char *nom_du_fichier);

/* Met /FF/D9 (EOI) en binaire écrit à la fin du fichier
ferme l'image l'image 
Après cette fonction, l'image JPEG est NORMALEMENT lisible
*/
void fin_jpeg(FILE *fichier);

/*Les marqueurs : à utiliser pour rentrer ledit marqueur dans le fichier
On suit la structure telle qu'elle est donnée dans le pdf */
/* Pour APP */
void marqueur_app(FILE *fichier);

