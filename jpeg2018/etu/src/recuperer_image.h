//
// Created by Vivien on 17/05/18
//

#ifndef RECUPERER_IMAGE
#define RECUPERER_IMAGE

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct Image {int couleur; // 0 si noir et blanc, 1 si couleur
                    uint32_t largeur;
                    uint32_t hauteur;
                    uint32_t valeur;
                    char *stream; // Le premier bit est stream[0], le i_ième est stream[i-1]. Pour observer ces bits, printf("%0hhx", stream[i])
                    size_t taille_stream;};


#include <stdlib.h>

/* Crée une structure Image du fichier rentré et la retourne. */
struct Image *initialisation(char *name_file);

/* Libère une image */
void free_image(struct Image *pic);

/* Prend le buffer, et renvoie la valeur entière qu'il contient */
uint32_t prendre_valeur(char *chaine);

/* Place dans le buffer tous les caractères dans le stream lecture jusqu'à la première
occurence du caractère retour.
Renvoie la taille du mot placé dans buffer */
size_t recuperer_jusqua_retour(FILE *lecture, char *buffer, char retour);

/* Place dans le buffer tous les caractères dans le stream lecture jusqu'à la fin.
Renvoie la taille du mot placé dans buffer */
size_t recuperer_jusqua_fin(FILE *lecture, char *buffer);

/* Réinitialise le buffer pour pouvoir y stoquer la suite */
void remettre_zero(size_t indice, char *buffer);

/* Affiche les valeurs de l'image rentrée */
void afficher_pic(struct Image *pic);

#endif //RECUPERER_IMAGE
