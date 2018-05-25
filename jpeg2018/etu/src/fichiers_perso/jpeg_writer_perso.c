#include "jpeg_writer.h"


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
struct huff_table;

/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* Alloue et retourne une nouvelle structure jpeg_desc. */
extern struct jpeg_desc *jpeg_desc_create(void)
{
    struct jpeg_desc *jpeg = malloc(sizeof(jpeg_desc));
    jpeg->ppm_filename = NULL;
    jpeg->jpeg_filename = NULL;
    jpeg->sampling_factor = NULL;
    jpeg->htables = NULL;
    jpeg->qtables = NULL;
    jpeg->dct = NULL;
}

/*
    Détruit un jpeg_desc. Toute la mémoire qui lui est
    associée est libérée.
*/
extern void jpeg_desc_destroy(struct jpeg_desc *jpeg)
{
  if (jpeg->ppm_filename != NULL;) { free(jpeg->ppm_filename);}
  if (jpeg->jpeg_filename != NULL;) { free(jpeg->jpeg_filename);}
  if (jpeg->sampling_factor != NULL;) { free(jpeg->sampling_factor);}
  if (jpeg->htables != NULL;) { free(jpeg->htables);}
  if (jpeg->qtables != NULL;) { free(jpeg->qtables);}
  if (jpeg->dct != NULL;) { free(jpeg->dct);}
  free(jpeg);
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
    bitstream_write_nbits(jpeg->bistream, 0xffd8, 16, 1);

    // Application data : APPx 0xffe0
    bitstream_write_nbits(jpeg->bistream, 0xffe0, 16, 1);                       // marqueur APP0
    bitstream_write_nbits(jpeg->bistream, 16, 16, 0);                           // longueur 16 octets
    bitstream_write_nbits(jpeg->bistream, 'J', 8, 0);                           // JFIF
    bitstream_write_nbits(jpeg->bistream, 'F', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'I', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'F', 8, 0);
    bitstream_write_nbits(jpeg->bistream, '\0', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 1, 8, 0);
    bitstream_write_nbits(jpeg->bistream, 1, 8, 0);
    bitstream_write_nbits(jpeg->bistream, 0, 56, 0);                            // données pour le JFIF

    // Commentaire : COM 0xfffe
    bitstream_write_nbits(jpeg->bistream, 0xfffe, 16, 1);                       // marqueur COM
    bitstream_write_nbits(jpeg->bistream, 17, 16, 0);                           // longueur 17 octets
    bitstream_write_nbits(jpeg->bistream, 'm', 8, 0);                           // made by antonin
    bitstream_write_nbits(jpeg->bistream, 'a', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'd', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'e', 8, 0);
    bitstream_write_nbits(jpeg->bistream, ' ', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'b', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'y', 8, 0);
    bitstream_write_nbits(jpeg->bistream, ' ', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'A', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'n', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 't', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'o', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'n', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'i', 8, 0);
    bitstream_write_nbits(jpeg->bistream, 'n', 8, 0);

    // Define Quantization Table : DQT 0xffdb
    bitstream_flush(jpeg->bitstream);
    for (uint8_t iq = 0; iq < jpeg->nb_components; iq++) {
        bitstream_write_nbits(jpeg->bistream, 0xffdb, 16, 1);                   // marqueur DQT
        bitstream_write_nbits(jpeg->bistream, 67, 16, 0);                       // longueur 67 octets
        bitstream_write_nbits(jpeg->bistream, 0, 4, 0);                         // precision 8bits
        bitstream_write_nbits(jpeg->bistream, iq, 4, 0);                        // indice de la table
        for (uint8_t indice = 0; indice < 64; indice++) {                       // valeurs de la table
            bitstream_write_nbits(jpeg->bistream, jpeg->qtables[iq][indice], 8, 0);
        }
    }

    // Start Of Frame : SOFx 0xffc0
    bitstream_flush(jpeg->bitstream);
    bitstream_write_nbits(jpeg->bistream, 0xffc0, 16, 1);                       // marqueur SOF0
    bitstream_write_nbits(jpeg->bistream, 8 + 3*jpeg->nb_components, 16, 0);    // longueur 3N + 8 octets
    bitstream_write_nbits(jpeg->bistream, 8, 8, 0);                             // precision de 8
    bitstream_write_nbits(jpeg->bistream, jpeg->height, 16, 0);                 // hauteur
    bitstream_write_nbits(jpeg->bistream, jpeg->width, 16, 0);                  // largeur
    bitstream_write_nbits(jpeg->bistream, jpeg->nb_components, 8, 0);           // nombre de composantes couleur
    //TODO
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        bitstream_write_nbits(jpeg->bistream, ic, 8, 0);           //XXXXX      // identifiant de composante
        bitstream_write_nbits(jpeg->bistream, jpeg->sampling_factor[2*ic], 4, 0);// facteur d'échantillonnage horizontale
        bitstream_write_nbits(jpeg->bistream, jpeg->sampling_factor[2*ic + 1], 4, 0);// facteur d'échantillonnage verticale
        bitstream_write_nbits(jpeg->bistream, 1-ic/2, 8, 0);       //XXXXX      // table de quantification iq
    }

    // Define Huffman Table : DHT 0xffc4
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        for (uint8_t ia = 0; ia < 2; ia++) {
            // VOIR DANS HTABLES !!!!!
            
            uint8_t *table_length = huffman_table_get_length_vector(jpeg->htables[2*ic + ia]);
            uint8_t *table_symbol = huffman_table_get_symbols(jpeg->htables[2*ic + ia]);
            assert(longueur_totale <= 256);
            longueur_totale = longueur_totale/8 + 1;
            bitstream_write_nbits(jpeg->bistream, 0xffc4, 16, 1);               // marqueur DHT
            bitstream_write_nbits(jpeg->bistream, 19 + table_length, 16, 0);   // longueur 17 + L octets
            bitstream_write_nbits(jpeg->bistream, 0, 3, 0);                     // toujours 0
            bitstream_write_nbits(jpeg->bistream, ia, 1, 0);                    // 0 pour DC, 1 pour AC
            bitstream_write_nbits(jpeg->bistream, 2*ic + ia, 4, 0); //XXXXX     // indice de Huffman
            for (uint8_t longueur = 0; longueur < 16; longueur++) {
                bitstream_write_nbits(jpeg->bistream, table_length[longueur], 8, 0);// nombre de symboles de taille longueur
            }
            uint16_t indice_totale = 0;
            for (uint8_t longueur = 0; longueur < 16; longueur++) {
                for (uint8_t indice = 0; indice < table_length[longueur]; indice++) {
                    bitstream_write_nbits(jpeg->bistream, table_symbol[indice_totale], longueur, 0);   // nombre de symboles de taille longueur
                    indice_totale++;
                }
            }
        }
    }

    bitstream_flush(jpeg->bitstream);

    // Start Of Scan : SOS 0xffda
    bitstream_write_nbits(jpeg->bistream, 0xffda, 16, 1);                       // marqueur SOS
    bitstream_write_nbits(jpeg->bistream, 6 + 2*jpeg->nb_components, 16, 0);    // longueur 2N + 6 octets
    bitstream_write_nbits(jpeg->bistream, jpeg->nb_components, 8, 0);           // nombre de composantes couleur
    for (uint8_t ic = 0; ic < jpeg->nb_components; ic++) {
        bitstream_write_nbits(jpeg->bistream, ic, 8, 0);                        // ic
        bitstream_write_nbits(jpeg->bistream, 2*ic, 4, 0);                      // indice de la table de huffman pour DC
        bitstream_write_nbits(jpeg->bistream, 2*ic + 1, 4, 0);                  // indice de la table de huffman pour DC
    }
    bitstream_write_nbits(jpeg->bistream, 0, 8, 0);                             // Ss toujours 0
    bitstream_write_nbits(jpeg->bistream, 63, 8, 0);                            // Se toujours 63
    bitstream_write_nbits(jpeg->bistream, 0, 8, 0);                             // Se toujours 00

}

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
extern void jpeg_write_footer(struct jpeg_desc *jpeg)
{
    bitstream_flush(jpeg->bitstream);
    // Fin d'image : SOI 0xffd9
    bitstream_write_nbits(jpeg->bistream, 0xffd9, 16, 1);
}


/*********************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg_desc */
/*********************************************************/

/* Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc jpeg. */
extern void jpeg_desc_set_ppm_filename(struct jpeg_desc *jpeg,
                                       const char *ppm_filename)
{
    strcpy(jpeg->ppm_filename, ppm_filename);
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
    strcpy(jpeg->jpeg_filename, jpeg_filename);
    jpeg->bitstream = bitstream_create("jpeg_filename")
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
        perror("Number of color components must be set before setting sampling factors.\n")
        exit(1);
    } else {
        if (color_component >= jpeg->nb_components) {
            perror("Color component out of range.\n")
            exit(1);
        } else {
            jpeg->sampling_factor[2*cc + dir] = sampling_factor;
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
        perror("Number of color components must be set before setting Huffman's tables.\n")
        exit(1);
    } else {
        if (color_component >= jpeg->nb_components) {
            perror("Color component out of range.\n")
            exit(1);
        } else {
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
    return(jpeg->htables[2*cc + dir]);
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
        perror("Number of color components must be set before setting quantification tables.\n")
        exit(1);
    } else {
        if (color_component >= jpeg->nb_components) {
            perror("Color component out of range.\n")
            exit(1);
        } else {
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


/*
    Permet de choisir une implémentation de DCT à enregistrer dans le jpeg_desc.
 */
extern void jpeg_desc_set_dct(struct jpeg_desc *jpeg, void (*dct)(int16_t *))
{
    jpeg->dct = dct;
}


/*
    Retourne la fonction utilisée pour calculer la DCT sur un bloc 8x8.
*/
extern void *jpeg_desc_get_dct(struct jpeg_desc *jpeg)
{
    return(jpeg->dct);
}
