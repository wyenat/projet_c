#include <stdio.h>
#include "bitstream.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "huffman.h"
#include "qtables.h"

void ecrire_facteur(struct jpeg_desc *jpeg, int h1, int h2, int h3, int v1, int v2, int v3){
  int tab[] = {h1, v1, h2, v2, h3, v3};
  for (int cc = 0; cc<3; cc++){
    for (int dir = 0; dir<2; dir++){
        jpeg_desc_set_sampling_factor(jpeg, cc, dir, tab[2*cc + dir]);
        // printf("Le facteur %u pour la couleur %u, et la direction %u \n", jpeg_desc_get_sampling_factor(jpeg, cc, dir), cc, dir);
    }
  }
}

void ecrire_huffman(struct jpeg_desc *jpeg){
  struct huff_table *htable;
  uint8_t *nb_sym_per_len;
  uint8_t *symbols;
  uint8_t nb_symbols;
  for (enum color_component cc = 0; cc<3; cc++){
    for (enum sample_type sample_type=0; sample_type < 2; sample_type++){
      printf("Component : %d, Sample type : %d ! \n", cc, sample_type);
        nb_sym_per_len =  htables_nb_symb_per_lengths[sample_type][cc];
        symbols = htables_symbols[sample_type][cc];
        nb_symbols = htables_nb_symbols[sample_type][cc];
        htable = huffman_table_build( nb_sym_per_len, symbols, nb_symbols);
        jpeg_desc_set_huffman_table(jpeg, sample_type, cc, htable);
    }
  }
}

void ecrire_quantification(struct jpeg_desc *jpeg){
  jpeg_desc_set_quantization_table(jpeg, Y, compressed_Y_table);
  jpeg_desc_set_quantization_table(jpeg, Cb, compressed_CbCr_table);
  jpeg_desc_set_quantization_table(jpeg, Cr, compressed_CbCr_table);
}

void ecrire_entete(struct jpeg_desc *jpeg, int h1, int h2, int h3, int v1, int v2, int v3){
  ecrire_facteur(jpeg, h1, h2, h3, v1, v2, v3);
  ecrire_huffman(jpeg);
  ecrire_quantification(jpeg);
  jpeg_write_header(jpeg);
}