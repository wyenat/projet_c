//
// Created by Vivien on 17/05/18
//

#ifndef RECUPERER_IMAGE
#define RECUPERER_IMAGE

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern truct Image {int couleur,
                    uint32_t largeur,
                    uint32_t hauteur,
                    uint32_t valeur,
                    char *stream,
                    size_t taille_stream};


#endif //RECUPERER_IMAGE

#include <stdlib.h>

/* Crée une structure Image du fichier rentré et la retourne. */
extern struct Image *initialisation(char *name_file);

/* Libère une image */
extern void free_image(struct Image *pic);

/* Prend le buffer, et renvoie la valeur entière qu'il contient */
extern uint32_t prendre_valeur(char *chaine);

/* Place dans le buffer tous les caractères dans le stream lecture jusqu'à la première
occurence du caractère retour.
Renvoie la taille du mot placé dans buffer */
extern size_t recuperer_jusqua_retour(FILE *lecture, char *buffer, char retour);

/* Réinitialise le buffer pour pouvoir y stoquer la suite */
extern void remettre_zero(size_t indice, char *buffer);

/* Affiche les valeurs de l'image rentrée */
extern void afficher_pic(struct Image *pic);
