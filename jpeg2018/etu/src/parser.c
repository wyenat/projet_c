#include <stdio.h>
#include "recuperer_image.h"
#include "decoupage_MCU.h"
#include "RGB2YCbCr.h"
#include "zig_zag.h"
#include "quantification.h"
#include "Downsampling.h"
#include "ACDC.h"
#include "bitstream.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "huffman.h"
#include "jpeg.h"


int main( int argc, char * argv[] )
{ if (argc == 1) {
  perror("Aucun arguments rentré ! Tapez --h ou --help pour plus d'informations \n");
  exit(EXIT_FAILURE);
}
  int verbose = 1; // A remettre à 0 pour rendre le projet final
  int sample  = 0;
  char* noms_des_images[argc];
  char* renommage[argc];
  int nombre_de_renommage=0;
  int nombre_dimages = 0;
  uint8_t h1=1;
  uint8_t h2=1;
  uint8_t h3=1;
  uint8_t v1=1;
  uint8_t v2=1;
  uint8_t v3=1;
	for(int i=1; i<argc; i++) {
        if (strcmp(argv[i],"--h" ) == 0 || strcmp(argv[i], "--help") == 0 ) {
          printf("Voilà les options : \n \t --outfile=nomdufichier renomme la sortie en nomdufichier.jpg (ne supporte pas les espaces) \n");
          printf("\t --sample=h1xv1,h2xv2,h3xv3 : défini les composantes d'échantillonages pour les 3 couleurs \n");
          printf("\t --verbose : affiche le mode verbeux \n");
        }
        else if ( strncmp(argv[i], "--outfile=", 10) == 0 ) {
          if (strlen(argv[i]) == 10) {
            printf("Impossible de renommer le fichier si aucun nom n'est rentré, vérifiez qu'il n'y a pas d'espace entre le = et le nom du fichier \n Le fichier sera nom par défaut");
          } else {
            char* outfile = malloc(strlen(argv[i]));
            strncpy(outfile, argv[i]+10, strlen(argv[i])-10);
            renommage[nombre_de_renommage] = outfile;
            nombre_de_renommage++;
          }
        }
        else if ( strncmp(argv[i], "--sample=", 9) == 0 ) {
          sample = 1;
          h1 = atoi(&argv[i][9]);
          v1 = atoi(&argv[i][11]);
          h2 = atoi(&argv[i][13]);
          v2 = atoi(&argv[i][15]);
          h3 = atoi(&argv[i][17]);
          v3 = atoi(&argv[i][19]);
          if (h1 == 0 || v1 == 0 || h2 == 0 || v2 == 0 || h3 == 0 || v3 == 0) {
            perror("Une ou plusieurs composantes rentrée n'est pas un chiffre !");
            exit(EXIT_FAILURE);
          }
        }
        else if (strcmp(argv[i], "--verbose") == 0 ) {
          verbose = 1;
        }
        else if (strcmp(argv[i]+strlen(argv[i])-4, ".ppm") == 0 || strcmp(argv[i]+strlen(argv[i])-4, ".pgm") == 0  ) {
          noms_des_images[nombre_dimages] = argv[i];
          nombre_dimages++;
        }
        else {
          printf("Erreur dans les arguments, %s n'est pas reconnu ! \n Faire --h ou --help pour plus d'informations \n", argv[i]);
          exit(EXIT_FAILURE);
        }
  }
  if (verbose) { //Pour l'instant seul le verbose fait quelque chose
      printf("-- Mode verbose activé -- \n");
      if (sample) {
        printf("-- Mode sample activé -- \n ");
        printf("\t Les coefficients rentrés sont %dx%d,%dx%d,%dx%d \n", h1,v1,h2,v2,h3,v3);
      }
      for (int i=0; i<nombre_dimages; i++) {
        printf("\n \n -- Image %d / %d à traiter : %s -- \n \n ", i+1, nombre_dimages, noms_des_images[i]);
        if (i >= nombre_de_renommage) {
          char* buffer = malloc(strlen(noms_des_images[i])*sizeof(char));
          strncpy(buffer, noms_des_images[i], strlen(noms_des_images[i])-4);
          renommage[i] = buffer;
        }
        printf("\t %s  --> %s.jpg \n", noms_des_images[i], renommage[i] );

        printf("\t Création du jpeg \n");
        struct jpeg_desc *jpeg = jpeg_desc_create();
        // Si c'est un PPM
        jpeg_desc_set_ppm_filename(jpeg, noms_des_images[i]);
        printf("On ajoute %s dans le filename \n", jpeg_desc_get_ppm_filename(jpeg));
        jpeg_desc_set_jpeg_filename(jpeg, renommage[i]);
        printf("Le nom est changé en %s dans le filename \n",jpeg_desc_get_jpeg_filename(jpeg));

        printf("\n \n \n \t initialisation de l'image ! \n \n \n ");
        struct Image *pic = initialisation(noms_des_images[i]);
        // afficher_pic(pic);

        printf("\n \n \n \t Passage MCU8 ! \n \n \n");
        struct Image_MCU_8 *image = decoupe(pic, 1, 1);
        // afficher_image_8(image);


        // Partie écriture jpeg (kinda)
        uint32_t hauteur_n = image->hauteur*8*(image->MCUs[0]->hauteur);
        uint32_t largeur_n = image->largeur*8*(image->MCUs[0]->largeur);
        jpeg_desc_set_image_height(jpeg, hauteur_n);
        printf("La hauteur écrite dans le fichier est %u \n", jpeg_desc_get_image_height(jpeg));
        jpeg_desc_set_image_width(jpeg, largeur_n);
        printf("La largeur écrite dans le fichier est %u \n", jpeg_desc_get_image_width(jpeg));
        uint8_t nb_couleur = 1 + 2*image->couleur;
        jpeg_desc_set_nb_components(jpeg, nb_couleur);
        printf("Il y a %u couleurs \n",  jpeg_desc_get_nb_components(jpeg));



        //Ecriture de l'entête
        ecrire_entete(jpeg ,h1, h2, h3, v1, v2, v3);
        struct bitstream *bitstream_jpeg = jpeg_desc_get_bitstream(jpeg);
        printf("\n ==> %s \n",bitstream_jpeg);



        printf("\n \n \n \t Conversion de RGB à YCbCr ! \n \n \n");
        Image_RGB2YCbCr(image);
        // afficher_image_YCbCr(image);

        // printf("\n \n \n \t Compression des images (pour le moment on compresse en vertical et en horizontal) \n \n \n");
        // Image_downsampling(image, 1, 1);
        // afficher_image_compressee(image);
        // printf("Hauteur du MCU de l'image : %d\n", image->MCUs[0]->hauteur);
        // printf("Largeur du MCU de l'image : %d\n", image->MCUs[0]->largeur);

        printf("\n \n \n \t Conversion de YCbCr à DCT \n \n \n");
        // ATTENTION : image devient new_image !!!! (On a besoin d'en recréer une nouvelle vu q'uon change de type d'image.)
        struct Image_MCU_16 *new_image = Image_DCT(image);
        // afficher_image_DCT(new_image);

        printf("\n \n \n \t Zig Zag ! \n \n ");
        zig_zag_image(new_image);

        printf("\n \n \n \t Quantification ! \n \n ");
        quantifier_image(new_image);
        // afficher_image_DCT(new_image);

        printf("\n Tentative d'écriture en dehors de jpeg.c  \n");
        bitstream_write_nbits(bitstream_jpeg, 8, 8, 0);

        printf("\n \n \n \t ACDC ! \n \n");
        ACDC_me(new_image, bitstream_jpeg);
        //bitstream_flush(bitstream_jpeg);
        jpeg_write_footer(jpeg);

    }
  }
  else {
    printf("Vous n'êtes pas dans le mode verbose, il ne se passe donc rien pour l'instant ! =D \n Faire --verbose \n");
  }
  printf("\n");
}
