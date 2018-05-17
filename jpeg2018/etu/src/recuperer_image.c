 #include <stdlib.h>
 #include <stdint.h>
 #include <stdio.h>
 #include <string.h>

struct Image {
  int couleur;              //0 si en noir et blanc, 1 sinon.
  uint32_t largeur;
  uint32_t hauteur;
  uint32_t valeur;
  char *stream; // Le premier bit est stream[0], le i_ième est stream[i-1]. Pour observer ces bits, printf("%0hhx", stream[i])
  size_t taille_stream;
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
  while (caractere_courant != retour && !feof(lecture)) {
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

/* Libère une image */
void free_image(struct Image *pic){
  free(pic->stream);
  free(pic);
}

/* Affiche les valeurs de l'image rentrée */
void afficher_pic(struct Image *pic){
  printf("En couleur : %d \n", pic->couleur);
  printf("Dimentions : %d x %d \n", pic->hauteur, pic->largeur);
  printf("La ligne avec 255 affiche : %d \n", pic->valeur);
  printf("%zu octets utilisés : ", pic->taille_stream);
  for (size_t i=0; i<pic->taille_stream; i++) {printf("%0hhx ", pic->stream[i]);}
  printf("\n");
}

/* Crée une structure Image du fichier rentré et la retourne. */
struct Image initialisation(char *name_file){
  FILE *lecture;
  lecture = fopen(name_file, "r");
  char *buffer = malloc(sizeof(lecture));
  remettre_zero(sizeof(buffer), buffer);
  // initialisation de couleur
  size_t indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  int couleur = 1;
  if (buffer[1] == '5') {
    couleur = 0;
  }
  remettre_zero(indice, buffer);
  // initialisation de largeur
  indice = recuperer_jusqua_retour(lecture, buffer, ' ');
  uint32_t largeur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  // initialisation de hauteur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t hauteur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  // initialisation de valeur;
  indice = recuperer_jusqua_retour(lecture, buffer, '\n');
  uint32_t valeur = prendre_valeur(buffer);
  remettre_zero(indice, buffer);
  // initialisation de stream et de taille_stream
  size_t taille_stream = recuperer_jusqua_retour(lecture, buffer, '\n');
  //initialisation de struct image pic
  struct Image *pic = malloc(sizeof(couleur) + 3*32 + sizeof(taille_stream) + sizeof(buffer));
  pic->couleur = couleur;
  pic->largeur = largeur;
  pic->hauteur = hauteur;
  pic->valeur  = valeur;
  pic->taille_stream = taille_stream;
  pic->stream = buffer;
  free(buffer);
  return *pic;
}


 int main(/*int argc, char* name_file[]*/) {
   struct Image pic = initialisation("/user/2/matterv/TP/projet_c/jpeg2018/etu/images/invader.pgm");
   afficher_pic(&pic);
 }
