 #include <stdlib.h>
 #include <stdint.h>
 #include <stdio.h>
 #include <string.h>

struct Image {
  int couleur;              //0 si en noir et blanc, 1 sinon.
  uint32_t largeur;
  uint32_t hauteur;
  uint32_t valeur;
  char *stream[];
};

void remettre_zero(size_t indice, char *buffer){
  for (size_t i=0; i<= indice; i++){
    buffer[i] = '\0';
  }
}

struct Image initialisation(/*char name_file*/){
  FILE *lecture;
  lecture = fopen("/user/2/matterv/TP/projet_c/jpeg2018/etu/images/invader.pgm", "r");
  char buffer[sizeof(lecture)];
  // initialisation de couleur
  size_t indice = 0;
  char caractere_courant = fgetc(lecture);
  char retour = '\n';
  while (caractere_courant != retour) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  printf("%s \n", buffer);
  int couleur = 1;
  if (strcmp(&buffer, "P5") == 0) {
    couleur = 0;
  }
  printf("%d \n", couleur);
  remettre_zero(indice, &buffer);
  // initialisation de largeur
  indice = 0;
  caractere_courant = fgetc(lecture);
  retour = ' ';
  while (caractere_courant != retour) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  uint32_t largeur = atoi(buffer);
  remettre_zero(indice, &buffer);
  printf("%d \n", largeur);
  // initialisation de longueur;
  indice = 0;
  caractere_courant = fgetc(lecture);
  retour = '\n';
  while (caractere_courant != retour) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  uint32_t longueur = atoi(buffer);
  remettre_zero(indice, &buffer);
  printf("%d \n", longueur);
  // initialisation de stream 
}


 int main(/*int argc, char* name_file[]*/) {
   struct Image pic = initialisation(/**name_file[argc]*/);
   //pic = fopen("/user/2/matterv/TP/projet_c/jpeg2018/etu/src/decoupage_mcu", "rb");
 }
