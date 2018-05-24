//
// Created by antoine on 22/05/18.
//

#include <assert.h>
#include "huffman_perso.h"


/* Initialise une table de huffman (crée un noeud vide dans l'arbre) */

void init_noeud(struct huff_table * noeud, uint8_t etiquette, struct huff_table *parent, uint8_t profondeur)   {
    noeud->etiquette = etiquette;
    noeud->parent = parent;
    noeud->fils = NULL;
    noeud->profondeur = profondeur;
    noeud->nombre_symboles_fils = 0;
    noeud->a_ete_visite = 0;
    if (etiquette == NULL)  {
        noeud->est_sature = 0;
    }
    else    {
        noeud->est_sature = 1;
    }
}



void mise_a_jour_saturation(struct huff_table * noeud_courant) {
    if (noeud_courant->est_sature == 1) {
        if (noeud_courant->parent != NULL) {
            noeud_courant = noeud_courant->parent;
        }
        while (noeud_courant->fils[0]->est_sature * noeud_courant->fils[1]->est_sature == 1)    {
            noeud_courant->est_sature = 1;
            if (noeud_courant->parent != NULL) {
                noeud_courant = noeud_courant->parent;
            }
            else    {
                printf("L'arbre est completement saturé, PROBLEME !\n");
            }
        }
    }
}


void mise_a_jour_nombre_symboles_fils(struct huff_table * noeud_courant)  {
    while (noeud_courant->parent != NULL)   {
        noeud_courant = noeud_courant->parent;
        noeud_courant->nombre_symboles_fils ++;
    }
}




/* Recherche le prochain noeud d'etiquette différente de NULL à la même profondeur que le noeud donné en entrée.
 * Construit les noeuds qui ne sont pas encore construits pour pouvoir continuer l'exploration. */

struct huff_table * recherche_prochain_libre(struct huff_table *noeud_courant, uint8_t profondeur_souhaitee)  {
    if (noeud_courant->est_sature == 0 && noeud_courant->profondeur == profondeur_souhaitee) {
        // si le noeud courant satisfait aux conditions, on peut le renvoyer.

        return noeud_courant;
    }
    else    {
        uint8_t profondeur = noeud_courant->profondeur;
        if (noeud_courant->est_sature == 1) {
            // Si le noeud est saturé, on ne peut pas aller plus bas, donc dans tous les cas on remonte pour pouvoir cherche le prochain sommet libre.

            noeud_courant = noeud_courant->parent;
            profondeur --;

            // Tant que l'on n'est pas remonté à un point ou l'on a un fils de libre, on continue de remonter l'arbre.
            while (noeud_courant->est_sature == 1) {
                noeud_courant = noeud_courant->parent;
                profondeur--;
            }
        }

        // Une fois que l'on a trouvé un fils de libre, on redescend dans l'arbre jusqu'à atteindre la même profondeur que celle d'entrée
        while (profondeur < profondeur_souhaitee)  {
            /* L'un des deux fils au moins doit être vide, puisqu'on ne peut pas occuper tous les noueds d'une même
             profondeur s'il reste des symboles à placer dans l'arbre */

            if (noeud_courant->fils == NULL) {
                /* Si le noeud est une feuille mais que l'on est pas a la profondeur souhaitée, on crée ses fils.
                 * Ensuite on continue l'exploration en allant dans le fils gauche. */

                noeud_courant->fils = malloc(2* sizeof(struct huff_table));

                init_noeud(noeud_courant->fils[0], NULL, noeud_courant, profondeur);

                init_noeud(noeud_courant->fils[1], NULL, noeud_courant, profondeur);

                noeud_courant = noeud_courant->fils[0];

                profondeur ++;
            }
            else if (noeud_courant->fils[0]->est_sature == 0)   {
                noeud_courant = noeud_courant->fils[0];
                profondeur ++;
            }
            else if (noeud_courant->fils[1]->est_sature == 0)    {
                noeud_courant = noeud_courant->fils[1];
                profondeur ++;
            }
            else    {
                printf("ERREUR : Aucun des fils n'est libre (non saturé), il y a un problème dans l'algorithme !\n");
                return NULL;
            }
        }
        return noeud_courant;

    }

}



/*
    Construit un arbre de Huffman à partir d'une table de symboles comme
    présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
    contenant le nombre de symboles pour chaque longueur, symbols est le
    tableau des symboles ordonnés, et nb_symbols représente la taille du
    tableau symbols.
*/

struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths,
                                       uint8_t *symbols,
                                       uint8_t nb_symbols)  {

    // On initialise la racine de l'arbre.
    struct huff_table * table_huffman = malloc(sizeof(struct huff_table));
    init_noeud(table_huffman, NULL, NULL, 0);
    struct huff_table * noeud_courant = table_huffman;

    uint8_t profondeur = 1;
    uint16_t indice_symbole = 0;
    while (indice_symbole < nb_symbols)  {
        uint8_t indice_longueur = 0;
        while (indice_longueur < nb_symb_per_lengths[profondeur - 1])    {

            // On commence par acceder au premier noeud libre (non saturé) à la bonne profondeur.
            noeud_courant = recherche_prochain_libre(noeud_courant, profondeur);

            // On met la bonne étiquette sur ce noeud
            noeud_courant->etiquette = symbols[indice_symbole];
            noeud_courant->est_sature = 1;

            // On met a jour les indices, puisqu'on passe au symbole suivant.
            indice_longueur ++;
            indice_symbole ++;

            // On met à jour l'attribut est_sature des différents noeuds suite à l'ajout de l'etiquette non NULL dans l'arbre.
            mise_a_jour_saturation(noeud_courant);

            // On met également à jour l'attribut du nombre de fils de différents noeuds affectés par l'ajout.
            mise_a_jour_nombre_symboles_fils(noeud_courant);
        }
        profondeur ++;
    }
    return table_huffman;
}





/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
    valeur value. nbits est un paramètre de sortie permettant de stocker la
    longueur du chemin.
*/
uint32_t huffman_table_get_path(struct huff_table *ht,
                                       uint8_t value,
                                       uint8_t *nbits)  {
    uint16_t nombre_symboles_total = ht->nombre_symboles_fils;
    uint16_t nombre_symboles_visites = 0;
    uint32_t path = 0;
    while (nombre_symboles_visites < nombre_symboles_total) {
        if (ht->etiquette != NULL) {
            if (ht->etiquette == value) {
                return path;
            }
            else    {
                ht->a_ete_visite = 1;
                path = path / 10;
                *nbits = *nbits - (uint8_t)1;
                nombre_symboles_visites ++;

                ht = ht->parent;
            }
        }
        else    {
            if (ht->fils[0]->a_ete_visite != 1) {
                ht = ht->fils[0];
                path *= 10;
                *nbits = *nbits + (uint8_t)1;
            }
            else if (ht->fils[1]->a_ete_visite != 1)    {
                ht = ht->fils[1];
                path = (path + 1) * 10;
                *nbits = *nbits + (uint8_t)1;
            }
            else    {
                // Les deux fils ont déjà été completment visités, on remonte donc d'un cran
                ht->a_ete_visite = 1;
                path = path / 10;
                *nbits = *nbits - (uint8_t)1;

                ht = ht->parent;
            }
        }
    }
    if (nombre_symboles_visites == nombre_symboles_total)    {
        printf("Le noeud souhaité na pas été trouvé dans l'arbre...\n");
        return EXIT_FAILURE;
    }
}



/*
    Retourne le tableau des symboles associé à l'arbre de Huffman passé en
    paramètre.
*/
uint8_t *huffman_table_get_symbols(struct huff_table *ht)   {

    /* Il est important de noter que si l'arbre est bien construit, un parcours en profondeur fournit les symboles déjà
     * triés par ordre de profondeur, car on ajoute les symboles en priorité dans les fils gauches.
     * Il suffit donc de faire le parcours en profondeur et d'ajouter les symboles sans avoir à se soucier de l'ordre. */

    uint16_t nombre_symboles_total = ht->nombre_symboles_fils;
    uint16_t nombre_symboles_visites = 0;     // Servira également d'indice pour l'ajout dans symbols.


    uint8_t *symbols = malloc(nombre_symboles_total * sizeof(uint8_t));

    while   (nombre_symboles_visites < nombre_symboles_total)   {
        if (ht->etiquette != NULL)  {
            symbols[nombre_symboles_visites] = ht->etiquette;

            nombre_symboles_visites ++;
        }
        else    {
            if (ht->fils[0]->a_ete_visite != 10) {
                ht = ht->fils[0];
            }
            else if (ht->fils[1]->a_ete_visite != 10)    {
                ht = ht->fils[1];
            }
            else    {
                // Les deux fils ont déjà été completment visités, on remonte donc d'un cran
                ht->a_ete_visite = 10;

                ht = ht->parent;
            }
        }
    }
}


/*
    Retourne le tableau du nombre de symboles de chaque longueur associé à
    l'arbre de Huffman passé en paramètre.
*/
uint8_t *huffman_table_get_length_vector(struct huff_table *ht) {


    uint16_t nombre_symboles_total = ht->nombre_symboles_fils;
    uint16_t nombre_symboles_visites = 0;

    uint8_t *nb_symb_per_lengths = malloc(nombre_symboles_total * sizeof(uint8_t)); /* A priori on alloue trop de mémoire,
                                        mais avec la strucuture utilisée, on ne pe pas prévoir exactement combien de mémoire il faut allouer. */

    while   (nombre_symboles_visites < nombre_symboles_total)   {
        if (ht->etiquette != NULL)  {
            nb_symb_per_lengths[(ht->profondeur) -1] ++;

            nombre_symboles_visites ++;
        }
        else    {
            if (ht->fils[0]->a_ete_visite != 20) {
                ht = ht->fils[0];
            }
            else if (ht->fils[1]->a_ete_visite != 20)    {
                ht = ht->fils[1];
            }
            else    {
                // Les deux fils ont déjà été completment visités, on remonte donc d'un cran
                ht->a_ete_visite = 20;

                ht = ht->parent;
            }
        }
    }
}


void huffman_table_destroy(struct huff_table *ht)   {

    struct huff_table *racine = ht;

    while (! (racine->fils[0]->a_ete_visite == 200 && racine->fils[1]->a_ete_visite == 200)) {
        // Tant que les fils de la racine ne sont pas marqués comme étant à supprimer, on continue la suppression en profondeur.

        if (ht->fils == NULL){
            ht->a_ete_visite = 200;     // Il faut le libérer.
            ht = ht->parent;
        }
        else if (ht->fils[0]->a_ete_visite == 200 && ht->fils[1]->a_ete_visite == 200)  {
            free(ht->fils);

            ht->a_ete_visite = 200;     // Les fils ont été supprimés, il faut marquer le noeud comme étant lui-même à supprimer.

            ht = ht->parent;
        }
        else if (ht->fils[0]->a_ete_visite != 200)  {
            ht = ht->fils[0];
        }
        else    {       //  ht->fils[1]->a_ete_visite != 200
            ht = ht->fils[1];
        }
    }

    free(ht);
}


void afficher_table_huffman(struct huff_table *table_huffman)   {

    uint16_t nombre_symboles_total = table_huffman->nombre_symboles_fils;
    uint16_t nombre_symboles_visites = 0;

    uint8_t **symboles_par_profondeur = malloc((size_t) (16 * nombre_symboles_total * sizeof(uint8_t))); // Il y a au plus 2^16 sle nombre de symboles total à  chaque profondeur

    uint16_t *indices_par_profondeur = malloc(16 * sizeof(uint16_t)); // Il y a 16 profondeurs différentes

    for (int profondeur = 0; profondeur < 16; profondeur++) {
        // Boucle utilisée pour l'affichage, pour savoir si une liste est vide ou non.
        symboles_par_profondeur[profondeur][0] = NULL;
        symboles_par_profondeur[profondeur][1] = NULL;
    }

    while (nombre_symboles_visites < nombre_symboles_total) {

        if (table_huffman->etiquette != NULL)   {
            symboles_par_profondeur[(table_huffman->profondeur) -1][indices_par_profondeur[(table_huffman->profondeur) -1]] = table_huffman->etiquette;
            symboles_par_profondeur[(table_huffman->profondeur) -1][indices_par_profondeur[(table_huffman->profondeur)]] = NULL; // Utile pour savoir où s'arreter lors de l'affichage.

            indices_par_profondeur[(table_huffman->profondeur) -1] ++;

            nombre_symboles_visites ++;
        }
        else    {
            if (table_huffman->fils[0]->a_ete_visite == 42 && table_huffman->fils[1]->a_ete_visite == 42)  {

                table_huffman->a_ete_visite = 42;     // Les fils ont été visité, il faut marquer le noeud comme étant lui-même visité.

                table_huffman = table_huffman->parent;
            }
            else if (table_huffman->fils[0]->a_ete_visite != 42)  {
                table_huffman = table_huffman->fils[0];
            }
            else    {       //  table_huffman->fils[1]->a_ete_visite != 42
                table_huffman = table_huffman->fils[1];
            }
        }

    }
    // A ce stade là, les tableaux sont construits.Il ne reste plus qu'à les afficher.

    for (int profondeur = 0; profondeur < 16; profondeur++) {

        printf("Profondeur : %d\n", profondeur + 1);
        printf("\t[ ");
        int indice = 0;
        while (symboles_par_profondeur[profondeur][indice +1] != NULL)  {
            printf("%d, ", symboles_par_profondeur[profondeur][indice]);
            indice ++;
        }
        if (symboles_par_profondeur[profondeur][indice] == NULL)    {
            printf("]\n\n");
        }
        else    {
            printf("%d ]\n\n", symboles_par_profondeur[profondeur][indice]);
        }
    }
    free(symboles_par_profondeur);
    free(indices_par_profondeur);
}


int main(void) {

    uint8_t *nb_symb_per_lengths = malloc(16 * sizeof(uint8_t));
    uint8_t temp_length[16] = {3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    nb_symb_per_lengths = temp_length;

    uint8_t *symbols = malloc(4 * sizeof(uint8_t));
    uint8_t temp_symbols[4] = {1,2,3,4};
    symbols = temp_symbols;

    struct huff_table *table_huffman = huffman_table_build(nb_symb_per_lengths, symbols, 4);

    afficher_table_huffman(table_huffman);
}