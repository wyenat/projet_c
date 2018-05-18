 #include "recuperer_image.h"
 #include <stdlib.h>
 #include <stdint.h>
 #include <stdio.h>
 #include <string.h>

void remettre_zero(size_t indice, char *buffer){
  for (size_t i=0; i<= indice; i++){
    buffer[i] = '\0';
  }
}

size_t recuperer_jusqua_retour(FILE *lecture, char *buffer, char retour) {
  uint32_t indice = 0;
  char caractere_courant = fgetc(lecture);
  while (caractere_courant != retour && !feof(lecture)) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  size_t i = indice;
  return i;
}

size_t recuperer_jusqua_fin(FILE *lecture, char *buffer) {
  uint32_t indice = 0;
  char caractere_courant = fgetc(lecture);
  while (!feof(lecture)) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  size_t i = indice;
  return i;
}


uint32_t prendre_valeur(char *chaine){
  uint32_t valeur = atoi(&chaine[0]);
  return valeur;
}

void free_image(struct Image *pic){
  free(pic->stream);
  free(pic);
}

void afficher_pic(struct Image *pic){
  printf("En couleur : %d \n", pic->couleur);
  printf("Dimentions : %d x %d \n", pic->hauteur, pic->largeur);
  printf("La ligne avec 255 affiche : %d \n", pic->valeur);
  printf("%zu octets utilisés : ", pic->taille_stream);
  for (size_t i=0; i<pic->taille_stream; i++) {printf("%0hhx ", pic->stream[i]);}
  printf("\n");
}

struct Image *initialisation(char *name_file){
  FILE *lecture;
  lecture = fopen(name_file, "r");
  char *buffer = malloc(32*sizeof(char));
  remettre_zero(sizeof(buffer), buffer);
  // initialisation de couleur
  size_t indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  int couleur = 2;
  if (buffer[0]=='P' && buffer[1] == '5') {
    couleur = 0;
  }
  else {if (buffer[0]=='P' && buffer[1] == '6') {
    couleur = 1;
  }
  else {
    perror("Il n'y a ni P5 ni P6 dans l'entête, vérifiez que le fichier est bien un .ppm ou un .pgm ! \n");
    exit(EXIT_FAILURE);
  }}
  remettre_zero(indice, buffer);
  // initialisation de largeur
  indice = recuperer_jusqua_retour(lecture, buffer, ' ');
  uint32_t largeur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  if ( largeur == 0 ) {
    perror("Il n'y a pas un V valide dans V*H ! \n");
    exit(EXIT_FAILURE);
  }
  // initialisation de hauteur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t hauteur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  if ( hauteur == 0 ) {
    perror("Il n'y a pas un H valide dans V*H ! \n");
    exit(EXIT_FAILURE);
  }
  // initialisation de valeur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t valeur = prendre_valeur(buffer);
  if ( valeur != 255 ) {
    perror("Il n'y a pas 255 dans l'entête ! \n");
    exit(EXIT_FAILURE);
  }
  remettre_zero(indice, buffer);
  free(buffer);
  // initialisation de stream et de taille_stream
  buffer = malloc(sizeof(char)*hauteur*largeur*3);
  size_t taille_stream = recuperer_jusqua_fin(lecture, buffer);
  if (taille_stream != hauteur*largeur*(1+2*couleur)) {
    perror("Nombre de données incohérent : l'image est incomplète ou surchargée ! \n");
    exit(EXIT_FAILURE);
  }
  //initialisation de struct image pic
  struct Image *pic = malloc(sizeof(couleur) + 3*32 + sizeof(taille_stream) + sizeof(buffer));
  pic->couleur = couleur;
  pic->largeur = largeur;
  pic->hauteur = hauteur;
  pic->valeur  = valeur;
  pic->taille_stream = taille_stream;
  pic->stream = buffer;
  fclose(lecture);
  return pic;
}


 int main(/*int argc, char* name_file[]*/) {
   struct Image *pic = initialisation("../images/zig-zag.ppm");
   //afficher_pic(pic);
   free_image(pic);
  //  pic = initialisation("../images/shaun_the_sheep.ppm");
  //  afficher_pic(pic);
  //  free_image(pic);
 }
