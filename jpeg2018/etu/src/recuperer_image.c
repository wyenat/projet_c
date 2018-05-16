 #include <stdlib.h>
 #include <stdint.h>
 #include <stdio.h>

struct Image {
  int couleur;              //0 si en noir et blanc, 1 sinon.
  uint32_t largeur;
  uint32_t hauteur;
  uint32_t valeur;
  char *stream[];
};

struct Image initialisation(char name_file){
  FILE *lecture;
  lecture = fopen(&name_file, "rb");
  char caractere_courant;
  caractere_courant=fgetc(lecture));
 }
 return picture;
}

 int main(int argc, char* name_file[]) {
   struct Image pic = initialisation(*name_file[argc]);
   //pic = fopen("/user/2/matterv/TP/projet_c/jpeg2018/etu/src/decoupage_mcu", "rb");
 }
