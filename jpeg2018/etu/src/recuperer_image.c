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

/* Réinitialise le buffer pour pouvoir y stoquer la suite */
void remettre_zero(size_t indice, char *buffer){
  for (size_t i=0; i<= indice; i++){
    buffer[i] = '\0';
  }
}

/* Place dans le buffer tous les caractères dans le stream lecture jusqu'à la première
occurence du caractère retour.
Renvoie la taille du mot placé dans buffer */
size_t recuperer_jusqua_retour(FILE *lecture, char *buffer, char retour) {
  size_t indice = 0;
  char caractere_courant = fgetc(lecture);
  while (caractere_courant != retour) {
    buffer[indice] = caractere_courant;
    caractere_courant = fgetc(lecture);
    indice++;
  }
  return indice;
}

/* Prend le buffer, et renvoie la valeur entière qu'il contient */
uint32_t prendre_valeur(char *chaine){
  uint32_t valeur = atoi(&chaine[0]);
  return valeur;
}

struct Image initialisation(/*char name_file*/){
  FILE *lecture;
  lecture = fopen("/user/2/matterv/TP/projet_c/jpeg2018/etu/images/invader.pgm", "r");
  char *buffer = malloc(sizeof(lecture));
  printf("Buffer initial : %s \n", buffer);
  remettre_zero(sizeof(buffer), buffer);
  // initialisation de couleur
  size_t indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  int couleur = 1;
  if (buffer[1] == '5') {
    couleur = 0;
  }
  printf("%d \n", couleur);
  remettre_zero(indice, buffer);
  // initialisation de largeur
  indice = recuperer_jusqua_retour(lecture, buffer, ' ');
  uint32_t largeur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  printf("%d \n", largeur);
  // initialisation de longueur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t longueur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  printf("%d \n", longueur);
  // initialisation de valeur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t valeur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  printf("%d \n", valeur);
  // initialisation de stream

}


 int main(/*int argc, char* name_file[]*/) {
   struct Image pic = initialisation(/**name_file[argc]*/);
   //pic = fopen("/user/2/matterv/TP/projet_c/jpeg2018/etu/src/decoupage_mcu", "rb");
 }
