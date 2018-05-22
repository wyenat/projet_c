#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"
#include "DCT.h"

//Je ne comprends pas bien comment gerer le byte stuffing mais sinon tout fonctionne.
//Tout le module ne fait pour l'instant que de l'affichage, il sortira des char* ou
//des ints par la suite selon ce que le bitstream prendra

// Partie magnétude, indice
uint8_t  obtenir_magnetude(int16_t entree){
  if (entree == 0){
    return 0;
  }
  int8_t  magnetude = log2(abs(entree))+1;
  return magnetude;
}

uint8_t  obtenir_indice(int16_t entree, int8_t  magnetude){
  if (magnetude == 0){
    return 0;
  }
  uint8_t indice = abs(entree);
  if (entree < 0){
    indice = pow(2, magnetude) - indice - 1;
  }
  return indice;
}

void affichage_ind_magn(int16_t entree){
  uint8_t  magnetude = obtenir_magnetude(entree);
  uint8_t  indice = obtenir_indice(entree, magnetude);
  printf("\n Le nombre %d a pour magnétude %d, pour indice %d \n", entree, magnetude, indice);
}

// Partie symbole symbole LRE
int compter_zero(int16_t *entree, int *courant){
  int nb_zero = 0;
  while (*courant < 64 && entree[*courant] == 0){
    // printf("Indice : %d On lit : %d \n", *courant, entree[*courant]);
    *courant = *courant + 1;
    nb_zero++;
  }
  if (*courant == 64) {
    return 64; //EOB
  } else if (nb_zero >= 16) {
    *courant = *courant - (nb_zero - 16);
    nb_zero = 16; //ZRL
  }
    return nb_zero;
  }

void ZRL(){
  // uint8_t zrl = 240;
  printf("/F0");
}

void EOB(){
  // uint8_t eob = 0;
  printf("/00\n");
}

void balise_std(int nb_zero, int valeur){
  uint8_t magnetude = obtenir_magnetude(valeur);
  if (magnetude == 0) {
    perror("On ne lit pas assez de 0 ! \n");
    exit(EXIT_FAILURE);
  }
  char *code = malloc(11*sizeof(char));
  code[0]='/';
  code[1] = hexme(nb_zero)[2];
  code[2] = hexme(magnetude)[2];
  if (strncmp(code, "/FF", 3)==0){ //byte_stuffing
    code[3] = '/';
    code[4] = '0';
    code[5] = '0';
  }
  printf("%s", code);
  // Partie écriture dans le fichier
  // uint16_t ecriture = pow(16,4)*nb_zero + magnetude;
  uint8_t indice = obtenir_indice(valeur, magnetude);
  printf("%s", binme_n(indice, magnetude));
  free(code);
}

void LRE(int16_t *entree){
  int courant = 0;
  while (courant < 64) {
    int nb_zero = compter_zero(entree, &courant);
    // affichage_ind_magn(entree[courant]);
    switch(nb_zero){
      case 16:
        ZRL();
        break;
      case 64:
        EOB();
        break;
      default:
        balise_std(nb_zero, entree[courant]);
        courant++;
        break;
    }
  }
}

//Partie adaptation à la structure;

int calcul_DC(int16_t *flux, int premier, int8_t DC){
    int somme=0;
    for (int i=0; i<64; i++){
      somme += flux[i];
    }
    somme = somme / 64;
    for (int i=0; i<64; i++){
      flux[i] -= somme;
    }
    if (premier == 1){
      somme -= DC;
    }
    return somme;
}

void ACDC_me(struct Image_MCU_16 *entree){
  int premier = 0;
  int8_t DC = 0;
  for (uint32_t hauteur=0; hauteur < entree->hauteur; hauteur++){
    for (uint32_t largeur=0; largeur < entree->largeur; largeur++){
      DC = calcul_DC(entree->MCUs[8*hauteur + largeur]->flux, premier, DC);
      uint8_t m = obtenir_magnetude(DC);
      uint8_t i = obtenir_indice(DC, i);
      printf("\n DC = %d \n", DC);
      LRE(entree->MCUs[8*hauteur + largeur]->flux);
      if (premier == 0){premier++;}
    }
  }
}
