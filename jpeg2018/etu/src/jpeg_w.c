 #include <stdio.h>

FILE *creer_fichier_jpeg(char *nom_du_fichier){
  FILE *fichier;
  fichier = fopen(nom_du_fichier, "wb+");
  int soi = 55551; //d8ff
  fwrite(&soi, 1, 2, fichier);
  return fichier;
}

void fin_jpeg(FILE *fichier){
  int eoi = 55807; //d9ff
  fwrite(&eoi, 1, 2, fichier);
  fclose(fichier);
}

void marqueur_app(FILE *fichier){
 int app = 57599; //e0 ff
 int longueur = 0016; // 0 10 
 char *jiff = "JFIF";
 int version = 1; //01, je ne sais pas si on doit le tester ou pas...
 int donnees_spe_non_traitees = 0; //0 0 0 0 0 0 0
 fwrite(&app, 1, 2, fichier);
 fwrite(&longueur, 1, 2, fichier);
 fputs(jiff, fichier);
 fwrite(&version, 1, 1, fichier);
 fwrite(&version, 1, 1, fichier);
 fwrite(&donnees_spe_non_traitees, 1, 7, fichier);
}

int main(){
 FILE *fichier;
 fichier = creer_fichier_jpeg("../images/Nouveau.jpg");
 marqueur_app(fichier);
 fin_jpeg(fichier);
}