#include "jpeg_writer_perso.h"
#include "huffman_perso.h"
#include <stdlib.h>
#include <stdio.h>


// /* Type énuméré représentant les composantes de couleur YCbCr. */
// enum color_component
// {
//     Y,
//     Cb,
//     Cr,
//     NB_COLOR_COMPONENTS
// };
//
// /*
//     Type énuméré représentant les types de composantes fréquentielles (DC ou
//     AC).
// */
// enum sample_type
// {
//     DC,
//     AC,
//     NB_SAMPLE_TYPES
// };
//
// /*
//     Type énuméré représentant la direction des facteurs d'échantillonnage (H
//     pour horizontal, V pour vertical).
// */
// enum direction
// {
//     H,
//     V,
//     NB_DIRECTIONS
// };
//
// /*
//     Type opaque contenant l'intégralité des informations nécessaires à
//     l'écriture de l'entête JPEG.
// */
// struct jpeg_desc;

/* Type opaque représentant un arbre de Huffman. */
//struct huff_table;

/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* Alloue et retourne une nouvelle structure jpeg_desc. */
extern struct jpeg_desc *jpeg_desc_create(void)
{
    struct jpeg_desc *jpeg = malloc(sizeof(struct jpeg_desc));
    jpeg->ppm_filename = NULL;
    jpeg->jpeg_filename = NULL;
    jpeg->sampling_factor = NULL;
    jpeg->htables = NULL;
    jpeg->qtables = NULL;
    return jpeg;
}

/*
    Détruit un jpeg_desc. Toute la mémoire qui lui est
    associée est libérée.
*/
extern void jpeg_desc_destroy(struct jpeg_desc *jpeg)
{
  if (jpeg->ppm_filename != NULL) {
      free(jpeg->ppm_filename);
  }
  if (jpeg->jpeg_filename != NULL) {
      free(jpeg->jpeg_filename);
  }
  if (jpeg->sampling_factor != NULL) {
      free(jpeg->sampling_factor);
  }
  if (jpeg->htables != NULL) {
      jpeg_huffman_table_destroy(jpeg);
      free(jpeg->htables);
  }
  if (jpeg->qtables != NULL) {
      free(jpeg->qtables);
  }
  free(jpeg);
}


/*
    Detruit les tables de Huffman du jpeg (liberation de la mémoire tout à la fin).
 */
void jpeg_huffman_table_destroy(struct jpeg_desc *jdesc){

    for (enum color_component cc = 0; cc<jdesc->nb_components; cc++){
        for (enum sample_type sample_type=0; sample_type < 2; sample_type++){
            huffman_table_destroy(jdesc->htables[2*cc + sample_type]);
        }
    }
}


/*
    Ecrit toute l'entête JPEG dans le fichier de sortie à partir des
    informations contenues dans le jpeg_desc passé en paramètre. En sortie, le
    bitstream est positionné juste après l'écriture de l'entête SOS, à
    l'emplacement du premier octet de données brutes à écrire.
*/
extern void jpeg_write_header(struct jpeg_desc *jpeg)
{
    assert(jpeg->ppm_filename != NULL);
    assert(jpeg->jpeg_filename != NULL);
    assert(jpeg->sampling_factor != NULL);
    assert(jpeg->htables != NULL);
    assert(jpeg->qtables != NULL);

    bitstream_flush(jpeg->bitstream);

    // Début d'image : SOI 0xffd8
    bitstream_write_nbits(jpeg->bitstream, 0xffd8, 16, 1);
    // printf("SOI écrit\n");

    // Application data : APPx 0xffe0
    bitstream_write_nbits(jpeg->bitstream, 0xffe0, 16, 1);                       // marqueur APP0
    bitstream_write_nbits(jpeg->bitstream, 16, 16, 0);                           // longueur 16 octets
    bitstream_write_nbits(jpeg->bitstream, 'J', 8, 0);                           // JFIF
    bitstream_write_nbits(jpeg->bitstream, 'F', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'I', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'F', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, '\0', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 1, 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 1, 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 0, 32, 0);                            // données pour le JFIF
    bitstream_write_nbits(jpeg->bitstream, 0, 24, 0);                            // données pour le JFIF
    // printf("APPx écrit\n");

    // Commentaire : COM 0xfffe
    bitstream_write_nbits(jpeg->bitstream, 0xfffe, 16, 1);                       // marqueur COM
    bitstream_write_nbits(jpeg->bitstream, 16, 16, 0);                           // longueur 17 octets
    bitstream_write_nbits(jpeg->bitstream, '<', 8, 0);                           // made by antonin
    bitstream_write_nbits(jpeg->bitstream, '5', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, ' ', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'l', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'e', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, ' ', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'p', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'r', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'o', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'j', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'e', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 't', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, ' ', 8, 0);
    bitstream_write_nbits(jpeg->bitstream, 'C', 8, 0);
    // printf("Commentaire écrit \n");

    // Define Quantization Table : DQT 0xffdb
    bitstream_flush(jpeg->bitstream);
    for (uint8_t iq = 0; iq < jpeg->nb_components; iq++) {
        if (iq == 2) {break;}                                                    // Il n'y a pas besoin de la troisième table
        bitstream_write_nbits(jpeg->bitstream, 0xffdb, 16, 1);                   // marqueur DQT
        bitstream_write_nbits(jpeg->bitstream, 67, 16, 0);                       // longueur 67 octets
        bitstream_write_nbits(jpeg->bitstream, 0, 4, 0);                         // precision 8bits
        bitstream_write_nbits(jpeg->bitstream, iq, 4, 0);                        // indice de la table
        for (uint8_t indice = 0; indice < 64; indice++) {                        // valeurs de la table
            bitstream_write_nbits(jpeg->bitstream, jpeg->qtables[iq][indice], 8, 0);
        }
    }
    // printf("Quantification table écrit\n");

    // Start Of Frame : SOFx 0xffc0
    bitstream_flush(jpeg->bitstream);
    bitstream_write_nbits(jpeg->bitstream, 0xffc0, 16, 1);                       // marqueur SOF0
    bitstream_write_nbits(jpeg->bitstream, 8 + 3*jpeg->nb_components, 16, 0);    // longueur 3N + 8 octets
    bitstream_write_nbits(jpeg->bitstream, 8, 8, 0);                             // precision de 8
    bitstream_write_nbits(jpeg->bitstream, jpeg->height, 16, 0);                 // hauteur
    bitstream_write_nbits(jpeg->bitstream, jpeg->width, 16, 0);                  // largeur
    bitstream_write_nbits(jpeg->bitstream, jpeg->nb_components, 8, 0);           // nombre de composantes couleur
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        bitstream_write_nbits(jpeg->bitstream, ic+1, 8, 0);                        // identifiant de composante
        bitstream_write_nbits(jpeg->bitstream, jpeg->sampling_factor[2*ic], 4, 0);// facteur d'échantillonnage horizontale
        bitstream_write_nbits(jpeg->bitstream, jpeg->sampling_factor[2*ic + 1], 4, 0);// facteur d'échantillonnage verticale
        uint8_t iq = 0;
        if (ic >= 1) {iq = 1;}                                                   // si ic vaut plus de 1 on est sur Cb ou Cr
        bitstream_write_nbits(jpeg->bitstream, iq, 8, 0);                        // table de quantification iq
    }
    // printf("Start of Frame écrit\n");

    // Define Huffman Table : DHT 0xffc4
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        if (ic == 2) {break;}
        for (uint8_t ia = 0; ia < 2; ia++) {
            // VOIR DANS HTABLES !!!!!
            uint8_t *table_length = huffman_table_get_length_vector(jpeg->htables[2*ic + ia]);
            uint8_t *table_symbol = huffman_table_get_symbols(jpeg->htables[2*ic + ia]);
            uint8_t longueur_totale = 0;
            for (uint8_t longueur = 0; longueur < 16; longueur++) {
                longueur_totale += table_length[longueur];
            }
            bitstream_write_nbits(jpeg->bitstream, 0xffc4, 16, 1);               // marqueur DHT
            bitstream_write_nbits(jpeg->bitstream, 19 + longueur_totale, 16, 0); // longueur 19+L octets
            bitstream_write_nbits(jpeg->bitstream, 0, 3, 0);                     // toujours 0
            bitstream_write_nbits(jpeg->bitstream, ia, 1, 0);                    // 0 pour DC, 1 pour AC
            uint8_t ih = 0;
            if (ic >= 1) {ih = 1;}                                               // si ic vaut plus de 1 on est sur Cb ou Cr
            bitstream_write_nbits(jpeg->bitstream, ih, 4, 0);                    // indice de Huffman
            for (uint8_t longueur = 0; longueur < 16; longueur++) {
                bitstream_write_nbits(jpeg->bitstream, table_length[longueur], 8, 0);// nombre de symboles de taille longueur
            }
            for (uint8_t indice = 0; indice < longueur_totale; indice++) {
                bitstream_write_nbits(jpeg->bitstream, table_symbol[indice], 8, 0);// symbole numéro indice
            }
            free(table_length);
            free(table_symbol);
        }
    }
    // printf("Huffman table écrit\n");


    // Start Of Scan : SOS 0xffda
    bitstream_write_nbits(jpeg->bitstream, 0xffda, 16, 1);                       // marqueur SOS
    bitstream_write_nbits(jpeg->bitstream, 6 + 2*jpeg->nb_components, 16, 0);    // longueur 2N + 6 octets
    bitstream_write_nbits(jpeg->bitstream, jpeg->nb_components, 8, 0);           // nombre de composantes couleur
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        bitstream_write_nbits(jpeg->bitstream, ic+1, 8, 0);                      // ic
        uint8_t iq = 0;
        if (ic >= 1) {iq = 1;}                                                   // si ic vaut plus de 1 on est sur Cb ou Cr
        bitstream_write_nbits(jpeg->bitstream, iq, 4, 0);                        // indice de la table de huffman pour DC
        bitstream_write_nbits(jpeg->bitstream, iq, 4, 0);                        // indice de la table de huffman pour AC
    }
    bitstream_write_nbits(jpeg->bitstream, 0, 8, 0);                             // Ss toujours 0
    bitstream_write_nbits(jpeg->bitstream, 63, 8, 0);                            // Se toujours 63
    bitstream_write_nbits(jpeg->bitstream, 0, 8, 0);                             // Se toujours 00
    // printf("Start of Scan écrit\n");
}

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
extern void jpeg_write_footer(struct jpeg_desc *jpeg)
{
    bitstream_flush(jpeg->bitstream);
    // Fin d'image : SOI 0xffd9
    bitstream_write_nbits(jpeg->bitstream, 0xffd9, 16, 1);
}


/*********************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg_desc */
/*********************************************************/

/* Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc jpeg. */
extern void jpeg_desc_set_ppm_filename(struct jpeg_desc *jpeg,
                                       const char *ppm_filename)
{
    size_t taille = strlen(ppm_filename)+1;
    jpeg->ppm_filename = malloc(taille*sizeof(char));
    strncpy(jpeg->ppm_filename, ppm_filename, taille);
}


/* Retourne le nom de fichier PPM lu dans le jpeg_desc jpeg. */
extern char *jpeg_desc_get_ppm_filename(struct jpeg_desc *jpeg)
{
    return(jpeg->ppm_filename);
}


/* Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc jpeg. */
extern void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jpeg,
                                        const char *jpeg_filename)
{
    size_t taille = strlen(jpeg_filename)+1;
    jpeg->jpeg_filename = malloc(taille*sizeof(char));
    strncpy(jpeg->jpeg_filename, jpeg_filename, taille);
    jpeg->bitstream = bitstream_create(jpeg->jpeg_filename);
}


/* Retourne le nom du fichier de sortie lu depuis le jpeg_desc jpeg. */
extern char *jpeg_desc_get_jpeg_filename(struct jpeg_desc *jpeg)
{
    return(jpeg->jpeg_filename);
}


/*
    Ecrit la hauteur en nombre de pixels de l'image traitée image_height dans
    le jpeg_desc jpeg.
*/
extern void jpeg_desc_set_image_height(struct jpeg_desc *jpeg,
                                       uint32_t image_height)
{
    jpeg->height = image_height;
}


/*
    Retourne la hauteur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jpeg.
*/
extern uint32_t jpeg_desc_get_image_height(struct jpeg_desc *jpeg)
{
    return(jpeg->height);
}


/*
    Ecrit la largeur en nombre de pixels de l'image traitée image_width dans le
    jpeg_desc jpeg.
*/
extern void jpeg_desc_set_image_width(struct jpeg_desc *jpeg,
                                      uint32_t image_width)
{
    jpeg->width = image_width;
}


/*
    Retourne la largeur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jpeg.
*/
extern uint32_t jpeg_desc_get_image_width(struct jpeg_desc *jpeg)
{
    return(jpeg->width);
}



/*
    Ecrit le nombre de composantes de couleur de l'image traitée nb_components
    dans le jpeg_desc jpeg.
*/
extern void jpeg_desc_set_nb_components(struct jpeg_desc *jpeg,
                                        uint8_t nb_components)
{
    jpeg->nb_components = nb_components;
    jpeg->sampling_factor = malloc(2*nb_components*sizeof(uint8_t));
    assert(jpeg->sampling_factor != NULL);
    jpeg->htables = malloc(2*nb_components*sizeof(struct huff_table *));
    assert(jpeg->htables != NULL);
    jpeg->qtables = malloc(nb_components*sizeof(uint8_t *));
    assert(jpeg->qtables != NULL);
}


/*
    Retourne le nombre de composantes de couleur de l'image traitée lu à partir
    du jpeg_desc jpeg.
*/
extern uint8_t jpeg_desc_get_nb_components(struct jpeg_desc *jpeg)
{
    return(jpeg->nb_components);
}


/*
    Retourne le bitstream associé au fichier de sortie enregistré dans le
    jpeg_desc.
*/
extern struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jpeg)
{
    return(jpeg->bitstream);
}


/*
    Ecrit dans le jpeg_desc jpeg le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
*/
extern void jpeg_desc_set_sampling_factor(struct jpeg_desc *jpeg,
                                          enum color_component cc,
                                          enum direction dir,
                                          uint8_t sampling_factor)
{
    if (jpeg->sampling_factor == NULL) {
        perror("Number of color components must be set before setting sampling factors.\n");
        exit(1);
    } else {
        if (jpeg->nb_components != 1) {
            // printf("Sampling factor color %u, direction %u SET\n", cc, dir);
            jpeg->sampling_factor[2*cc + dir] = sampling_factor;
        } else {
            // printf("Sampling factor grey, direction %u SET\n", dir);
            jpeg->sampling_factor[dir] = sampling_factor;
        }
    }
}


/*
    Retourne le facteur d'échantillonnage utilisé pour la composante de couleur
    cc et la direction dir, lu à partir du jpeg_desc jpeg.
*/
extern uint8_t jpeg_desc_get_sampling_factor(struct jpeg_desc *jpeg,
                                             enum color_component cc,
                                             enum direction dir)
{
    return(jpeg->sampling_factor[2*cc + dir]);
}


/*
    Ecrit dans le jpeg_desc jpeg la table de Huffman huff_table à utiliser
    pour encoder les données de la composante fréquentielle acdc pour la
    composante de couleur cc.
*/
extern void jpeg_desc_set_huffman_table(struct jpeg_desc *jpeg,
                                        enum sample_type acdc,
                                        enum color_component cc,
                                        struct huff_table *htable)
{
    if (jpeg->htables == NULL) {
        perror("Number of color components must be set before setting Huffman's tables.\n");
        exit(1);
    } else {
        if (cc < jpeg->nb_components) {
            jpeg->htables[2*cc + acdc] = htable;
        }
    }
}


/*
    Retourne un pointeur vers la table de Huffman utilisée pour encoder les
    données de la composante fréquentielle acdc pour la composante de couleur
    cc, lue à partir du jpeg_desc jpeg.
*/
extern struct huff_table *jpeg_desc_get_huffman_table(struct jpeg_desc *jpeg,
                                                      enum sample_type acdc,
                                                      enum color_component cc)
{
    return(jpeg->htables[2*cc + acdc]);
}



/*
    Ecrit dans le jpeg_desc jpeg la table de quantification qtable à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
extern void jpeg_desc_set_quantization_table(struct jpeg_desc *jpeg,
                                             enum color_component cc,
                                             uint8_t *qtable)
{
    if (jpeg->qtables == NULL) {
        perror("Number of color components must be set before setting quantification tables.\n");
        exit(1);
    } else {
        if (cc < jpeg->nb_components) {
            jpeg->qtables[cc] = qtable;
        }
    }
}


/*
    Retourne la table de quantification associée à la composante de couleur
    cc dans le jpeg_desc jpeg.
*/
extern uint8_t *jpeg_desc_get_quantization_table(struct jpeg_desc *jpeg,
                                                 enum color_component cc)
{
    return(jpeg->qtables[cc]);
}
