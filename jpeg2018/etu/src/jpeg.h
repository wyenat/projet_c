#include <stdio.h>
#include "bitstream.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "huffman.h"

/* Ecrit les facteurs de sampling dans le jpeg */
void ecrire_facteur(struct jpeg_desc *jpeg, int h1, int h2, int h3, int v1, int v2, int v3);

/* Ecrit les tables de Huffman dans le jpeg */
void ecrire_huffman(struct jpeg_desc *jpeg);

/* Ecrit les tables de quantification dans le jpeg */
void ecrire_quantification(struct jpeg_desc *jpeg);

/* Ecrit les informations qui figurent au tout début de l'entête */
void ecrire_debut(struct jpeg_desc *jpeg, char *nom_entree, char *nom_sortie, uint32_t hauteur_n, uint32_t largeur_n, uint8_t nb_couleur);

/* Ecrit toutes les entêtes au-dessus, puis les écrits dans le jpeg, et renvoit le pointeur vers le bitstream */
struct bitstream *ecrire_entete(struct jpeg_desc *jpeg, char *nom_entree, char *nom_sortie, uint32_t hauteur_n, uint32_t largeur_n, uint8_t nb_couleur, int h1, int h2, int h3, int v1, int v2, int v3);
