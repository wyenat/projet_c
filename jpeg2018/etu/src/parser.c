#include <stdio.h>
#include "recuperer_image.h"
#include "decoupage_MCU.h"
#include "jpeg_writer_prof.h"
#include "RGB2YCbCr.h"
#include "zig_zag.h"
#include "DCT.h"
#include "quantification.h"

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
  int h1=1;
  int h2=1;
  int h3=1;
  int v1=1;
  int v2=1;
  int v3=1;
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
        
        printf("\n \n \n \t initialisation de l'image ! \n \n \n ");
        struct Image *pic = initialisation(noms_des_images[i]);
        afficher_pic(pic);
        
        printf("\n \n \n \t Passage MCU8 ! \n \n \n");
        struct Image_MCU_8 *image = decoupe(pic, 1, 1);
        afficher_image_8(image);

        printf("\n \n \n \t Conversion de RGB à YCbCr ! \n \n \n");
        Image_RGB2YCbCr(image);
        afficher_image_8(image); //L'affichage est pas bon mais le vecteur si ! ;)
        
        printf("\n \n \n \t DCT : Dinosaures Coquins près de chez Toi! \n \n \n");
        // on manipule un struct Image_MCU_8 *, et on veut ressortir un void. 
        
        printf("\n \n \n \t Zig Zag me up daddy ! \n \n ");
        //Ne marche toujours pas des masses hihihi mon adaptation est pas terrible je crois.
        zag_zig(image);
        afficher_image_8(image);
        }
    }
  else {
    printf("Vous n'êtes pas dans le mode verbose, il ne se passe donc rien pour l'instant ! =D \n Faire --verbose \n");
  }
}
