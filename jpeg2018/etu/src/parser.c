#include <stdio.h>
#include "recuperer_image.h"
#include "decoupage_MCU.h"
#include "RGB2YCbCr.h"
#include "zig_zag.h"
#include "quantification.h"
#include "Downsampling.h"
#include "ACDC.h"
#include "bitstream_perso.h"
#include "jpeg_writer_perso.h"
#include "htables.h"
#include "huffman_perso.h"
#include "jpeg.h"


int main( int argc, char * argv[] )
{   if (argc == 1) {
  perror("Aucun arguments rentré ! Tapez --h ou --help pour plus d'informations \n");
  exit(EXIT_FAILURE);
}
  int verbose = 0;
  int sample  = 0;
  int force   = 0;
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
          printf("\t --force : crée le fichier même s'il existe déjà \n \t ATTENTION ! LA SUPPRESSION DU FICHIER ORIGINAL EST DEFINITIVE  \n");
        }
        else if ( strncmp(argv[i], "--outfile=", 10) == 0 ) {
          if (strlen(argv[i]) == 10) {
            printf("Impossible de renommer le fichier si aucun nom n'est rentré, vérifiez qu'il n'y a pas d'espace entre le = et le nom du fichier \n");
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
        else if (strcmp(argv[i], "--force") == 0 ) {
            force = 1;
        }
        else {
          printf("Erreur dans les arguments, %s n'est pas reconnu ! \n Faire --h ou --help pour plus d'informations \n", argv[i]);
          exit(EXIT_FAILURE);
        }
  }
  if (verbose) {
      printf("-- Mode verbose activé -- \n");
      if (sample) {
        printf("-- Mode sample activé -- \n ");
        printf("\t Les coefficients rentrés sont %dx%d,%dx%d,%dx%d \n", h1,v1,h2,v2,h3,v3);
      }
    }
      for (int i=0; i<nombre_dimages; i++) {
        printf("\n \n -- Image %d / %d à traiter : %s -- \n \n ", i+1, nombre_dimages, noms_des_images[i]);
        if (i >= nombre_de_renommage) {
          char* buffer = malloc(strlen((noms_des_images[i])+4)*sizeof(char));
          strncpy(buffer, noms_des_images[i], strlen(noms_des_images[i])-4);
          strcat(buffer, ".jpg");
          renommage[i] = buffer;
          printf("%s \n", buffer);
        }
        if (verbose){
          printf("\t %s  --> %s \n", noms_des_images[i], renommage[i] );
          printf("\t Création du jpeg \n");
        }

        // Vérifie que l'image de sortie n'existe pas
        FILE *fichier = NULL;
        fichier = fopen(renommage[i],  "r");
        if (fichier != NULL && force == 0){
            perror("Le fichier existe déjà ! \n Activez le mode --force si vous voulez l'écraser.\n");
            continue;
        }

        // Récupération des données dans l'images
        struct Image *pic = initialisation(noms_des_images[i]);
        if (verbose){
          printf("\n \n \n \t initialisation de l'image ! \n \n \n ");
          afficher_pic(pic);
        }
        if (pic->couleur == 0){
            h1 = 1;
            h2 = 1;
            h3 = 1;
            v1 = 1;
            v2 = 1;
            v3 = 1;
        }


        // Passage en MCU
        struct Image_MCU_8 *image = decoupe(pic, h1, v1);
        if (verbose){
          printf("\n \n \n \t Passage MCU8 ! \n \n \n");
          afficher_image_8(image);
        }


        // Création de l'entête jpeg
        struct jpeg_desc *jpeg = jpeg_desc_create();
        struct bitstream *bitstream_jpeg = ecrire_entete(jpeg ,noms_des_images[i], renommage[i], image->hauteur*8*(image->MCUs[0]->hauteur),  image->largeur*8*(image->MCUs[0]->largeur),1 + 2*image->couleur, h1, h2, h3, v1, v2, v3);
        free(renommage[i]);

        // Passage de RGB à YCbCr
        Image_RGB2YCbCr(image);
        if (verbose){
          printf("\n \n \n \t Conversion de RGB à YCbCr ! \n \n \n");
          afficher_image_YCbCr(image);
        }


        // Sous-échantillonages
        Image_downsampling(image, h1/h3 - 1, v1/v3 - 1);
        if (verbose){
          printf(" \n \n \n \t Sous-échantillonage ! \n \n \n");
          printf("Hauteur du MCU de l'image : %d\n", image->MCUs[0]->hauteur);
          printf("Largeur du MCU de l'image : %d\n", image->MCUs[0]->largeur);
          afficher_image_compressee(image);
        }


        // Discrete Cosin Transform
        struct Image_MCU_16 *new_image = Image_DCT(image);
        if (verbose){
          printf("\n \n \n \t Conversion de YCbCr à DCT \n \n \n");
          afficher_image_DCT(new_image);
        }


        // Zig Zag
        zig_zag_image(new_image);
        if (verbose){
          printf("\n \n \n \t Zig Zag ! \n \n ");
          afficher_image_DCT(new_image);
        }


        // Quantification
        quantifier_image(new_image);
        if (verbose){
          printf("\n \n \n \t Quantification ! \n \n ");
          afficher_image_DCT(new_image);
        }


        // Compression de l'image, écriture dans le flux
        if (verbose){
        printf("\n \n \n \t ACDC ! \n \n");
        }
        ACDC_me(new_image, bitstream_jpeg, jpeg, verbose);




        // Libération des mémoires allouée.
        jpeg_write_footer(jpeg);
        // fclose(fichier);
        jpeg_desc_destroy(jpeg);
        Image_destroy(new_image);
        bitstream_destroy(bitstream_jpeg);
        printf("Image traitée \n");
    }
  }
