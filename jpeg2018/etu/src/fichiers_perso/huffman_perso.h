//
// Created by antoine on 22/05/18.
//

#ifndef ETU_HUFFMAN_PERSO_H
#define ETU_HUFFMAN_PERSO_H


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Structure d'arbre de Huffman
struct huff_table   {
    /* Valeur du noeud, NULL si ce n'est pas une feuille. */
    uint8_t *etiquette;

    /* entier valant 0 ou 1, qui indique si un noeud est saturé ou non. Utilisé pour la recherche du prochain noeud libre. */
    uint8_t est_sature;

    /* Indique la profondeur du noeud. */
    uint8_t profondeur;

    /* Indique le nombre de symboles des fils du noeud courant. (nombre de feuilles des sous-arbres fils) */
    uint16_t nombre_symboles_fils;

    /* Indique si un noeud a été visité. Utilisé dans la recherche d'un élément. ATTENTION : dans la fonction de
     * recherche d'une valeur dans un noeud, a_ete_visite vaut 1 si le noeud a déjà été visité. Dans la fonction
     * de construction des tableaux de Huffman à partir de l'arbre, a_ete_visite vaut 10 si le noeud (ou toutes les feuilles
     * du sous-arbre courant) a deja été ajouté à la table.
     * Cela permet de n'utiliser qu'un seul argument pour les deux fonctions, et de ne pas être embêté par l'impact d'une
     * fonction sur une autre. */
    uint8_t a_ete_visite;

    // Pointeur vers les fils du noeud courrant. NULL si le noeud courant est une feuille.
    struct huff_table **fils;

    // Pointeur vers le parent.
    struct  huff_table *parent;
};




/*
    Initialise un noeud avec les valeurs d'etiquette, de parent et de profondeur données en paramètres.
*/
void init_noeud(struct huff_table * noeud, uint8_t etiquette, struct huff_table *parent, uint8_t profondeur);



/*
    Met à jour l'attribut "est_sature" des noeuds de l'arbre. Fonction appelée après l'ajout d'une étiquette non NULL dans l'arbre.
*/
void mise_a_jour_saturation(struct huff_table * noeud_courant);



/*
    A partir d'un noeud donné, recherche le prochain noeud dans l'arbre qui satsfait à la fois à la condition de non saturation
    et à la condition de profondeur (noeud libre et à la bonne profondeur).
*/
struct huff_table * recherche_prochain_libre(struct huff_table *noeud_courant, uint8_t profondeur_souhaitee);



/*
    Construit un arbre de Huffman à partir d'une table de symboles comme
    présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
    contenant le nombre de symboles pour chaque longueur, symbols est le
    tableau des symboles ordonnés, et nb_symbols représente la taille du
    tableau symbols.
*/
extern struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths,
                                              uint8_t *symbols,
                                              uint8_t nb_symbols);



/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
    valeur value. nbits est un paramètre de sortie permettant de stocker la
    longueur du chemin.
*/
extern uint32_t huffman_table_get_path(struct huff_table *ht,
                                       uint8_t value,
                                       uint8_t *nbits);


/*
    Retourne le tableau des symboles associé à l'arbre de Huffman passé en
    paramètre.
*/
extern uint8_t *huffman_table_get_symbols(struct huff_table *ht);



/*
    Retourne le tableau du nombre de symboles de chaque longueur associé à
    l'arbre de Huffman passé en paramètre.
*/
extern uint8_t *huffman_table_get_length_vector(struct huff_table *ht);



/*
    Détruit l'arbre de Huffman passé en paramètre et libère toute la mémoire
    qui lui est associée.
*/
extern void huffman_table_destroy(struct huff_table *ht);



/*
    Affichage de la table de Huffman : on affiche les tableaux de chaque profondeur.
*/
extern void afficher_table_huffman(struct huff_table *table_huffman);




#endif //ETU_HUFFMAN_PERSO_H
