#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "hex.h"


int8_t  obtenir_magnetude(int16_t entree){
  if (entree == 0){
    return 0;
  }
  int8_t  magnetude = log2(abs(entree))+1;
  return magnetude;
}

int8_t  obtenir_indice(int16_t entree, int8_t  magnetude){
  if (magnetude == 0){
    return 0;
  }
  int8_t  indice = abs(entree)-pow(2,magnetude-1);
  if (entree > 0){
    indice = indice + pow(2, magnetude-1);
  } else {
    indice = floor(fabs(indice - pow(2,magnetude-1)) - 1);
  }
  return indice;
}

void affichage(int16_t entree){
  int8_t  magnetude = obtenir_magnetude(entree);
  int8_t  indice = obtenir_indice(entree, magnetude);
  printf("Le nombre %d a pour magnétude %d, pour indice %d \n", entree, magnetude, indice);
}

// Partie symbole symbole LRE
int compter_zero(int *entree, int *courant){
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
  printf("/F0");
}

void EOB(){
  printf("/00");
}

void balise_std(nb_zero, valeur){
  int8_t magnetude = obtenir_magnetude(valeur);
  if (magnetude == 0) {
    perror("On ne lit pas assez de 0 ! \n");
    exit(EXIT_FAILURE);
  }
  char *code = malloc(11*sizeof(char));
  char *bitforts = hexme(nb_zero);
  char *faiblebit = hexme(magnetude);
  for (int i=0; i<3; i++){
    code[i] = bitforts[i];
  }
  for (int i=3; i<6; i++){
    code[i] = faiblebit[i-3];
  }
  printf("%s", code);
  free(faiblebit);
  free(bitforts);
  free(code);
  int8_t indice = obtenir_indice(valeur);
  
}

void LRE(int *entree){
  int courant = 0;
  while (courant < 64) {
    int nb_zero = compter_zero(entree, &courant);
    if (nb_zero == 16) {
      ZRL();
    } else if (nb_zero == 64) {
      EOB();
    } else {
      balise_std(nb_zero, entree[courant]);
      courant++;
    }
  }
}

int  main(){
  int *tab = malloc(8*64);
  for (int i=0; i<64; i++){
    tab[i] = 0;
  }
  tab[55] = 1;
  LRE(tab);
  printf("\n")
  for (int i=16; i<31; i++){
    printf("%d = %s \n", i, binme_n(obtenir_indice(i, 5), 5))
  }
}