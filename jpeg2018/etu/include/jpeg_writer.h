#ifndef JPEG_WRITER_H
#define JPEG_WRITER_H

#include <stdint.h>

/********************/
/* Types de données */
/********************/

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component
{
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant les types de composantes fréquentielles (DC ou
    AC).
*/
enum sample_type
{
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction
{
    H,
    V,
    NB_DIRECTIONS
};

/*
    Type opaque contenant l'intégralité des informations nécessaires à
    l'écriture de l'entête JPEG.
*/
struct jpeg_desc;

/* Type opaque représentant un arbre de Huffman. */
struct huff_table;

/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* Alloue et retourne une nouvelle structure jpeg_desc. */
extern struct jpeg_desc *jpeg_desc_create(void);

/*
    Détruit un jpeg_desc. Toute la mémoire qui lui est
    associée est libérée.
*/
extern void jpeg_desc_destroy(struct jpeg_desc *jdesc);

/*
    Ecrit toute l'entête JPEG dans le fichier de sortie à partir des
    informations contenues dans le jpeg_desc passé en paramètre. En sortie, le
    bitstream est positionné juste après l'écriture de l'entête SOS, à
    l'emplacement du premier octet de données brutes à écrire.
*/
extern void jpeg_write_header(struct jpeg_desc *jdesc);

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
extern void jpeg_write_footer(struct jpeg_desc *jdesc);


/*********************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg_desc */
/*********************************************************/

/* Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc jdesc. */
extern void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc,
                                       const char *ppm_filename);

/* Retourne le nom de fichier PPM lu dans le jpeg_desc jdesc. */
extern char *jpeg_desc_get_ppm_filename(struct jpeg_desc *jdesc);

/* Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc jdesc. */
extern void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc,
                                        const char *jpeg_filename);

/* Retourne le nom du fichier de sortie lu depuis le jpeg_desc jdesc. */
extern char *jpeg_desc_get_jpeg_filename(struct jpeg_desc *jdesc);

/*
    Ecrit la hauteur en nombre de pixels de l'image traitée image_height dans
    le jpeg_desc jdesc.
*/
extern void jpeg_desc_set_image_height(struct jpeg_desc *jdesc,
                                       uint32_t image_height);

/*
    Retourne la hauteur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jdesc.
*/
extern uint32_t jpeg_desc_get_image_height(struct jpeg_desc *jdesc);

/*
    Ecrit la largeur en nombre de pixels de l'image traitée image_width dans le
    jpeg_desc jdesc.
*/
extern void jpeg_desc_set_image_width(struct jpeg_desc *jdesc,
                                      uint32_t image_width);

/*
    Retourne la largeur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jdesc.
*/
extern uint32_t jpeg_desc_get_image_width(struct jpeg_desc *jdesc);


/*
    Ecrit le nombre de composantes de couleur de l'image traitée nb_components
    dans le jpeg_desc jdesc.
*/
extern void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc,
                                        uint8_t nb_components);

/*
    Retourne le nombre de composantes de couleur de l'image traitée lu à partir
    du jpeg_desc jdesc.
*/
extern uint8_t jpeg_desc_get_nb_components(struct jpeg_desc *jdesc);

/*
    Retourne le bitstream associé au fichier de sortie enregistré dans le
    jpeg_desc.
*/
extern struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc);

/*
    Ecrit dans le jpeg_desc jdesc le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
*/
extern void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc,
                                          enum color_component cc,
                                          enum direction dir,
                                          uint8_t sampling_factor);

/*
    Retourne le facteur d'échantillonnage utilisé pour la composante de couleur
    cc et la direction dir, lu à partir du jpeg_desc jdesc.
*/
extern uint8_t jpeg_desc_get_sampling_factor(struct jpeg_desc *jdesc,
                                             enum color_component cc,
                                             enum direction dir);

/*
    Ecrit dans le jpeg_desc jdesc la table de Huffman huff_table à utiliser
    pour encoder les données de la composante fréquentielle acdc pour la
    composante de couleur cc.
*/
extern void jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc,
                                        enum sample_type acdc,
                                        enum color_component cc,
                                        struct huff_table *htable);

/*
    Retourne un pointeur vers la table de Huffman utilisée pour encoder les
    données de la composante fréquentielle acdc pour la composante de couleur
    cc, lue à partir du jpeg_desc jdesc.
*/
extern struct huff_table *jpeg_desc_get_huffman_table(struct jpeg_desc *jdesc,
                                                      enum sample_type acdc,
                                                      enum color_component cc);

/*
    Ecrit dans le jpeg_desc jdesc la table de quantification qtable à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
extern void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc,
                                             enum color_component cc,
                                             uint8_t *qtable);

/*
    Retourne la table de quantification associée à la composante de couleur
    cc dans le jpeg_desc jdesc.
*/
extern uint8_t *jpeg_desc_get_quantization_table(struct jpeg_desc *jdesc,
                                                 enum color_component cc);

#endif /* JPEG_WRITER_H */
