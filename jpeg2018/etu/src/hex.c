//Module servant à convertir des int en hexa pour vérifier que je sors bien les
//bons trucs avec de les coder en hard

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


char calculer_coeff(int *entree,int puissance){
  int coeff = 0;
  int copie_entree = *entree;
  while (copie_entree - pow(16,puissance)*coeff > 0){
    coeff++;
  }
  if (copie_entree - pow(16,puissance)*coeff < 0){
    coeff = coeff-1;
  }
  *entree = *entree - pow(16, puissance)*(coeff);  
  switch (coeff){
    case 0:
      return '0';
      break;
    case 1:
      return '1';
      break;
    case 2:
      return '2';
      break;
    case 3:
      return '3';
      break;
    case 4:
      return '4';
      break;
    case 5:
      return '5';
      break;
    case 6:
      return '6';
      break;
    case 7:
      return '7';
      break;
    case 8:
      return '8';
      break;
    case 9:
      return '9';
      break;
    case 10:
      return 'A';
      break;
    case 11:
      return 'B';
      break;
    case 12:
      return 'C';
      break;
    case 13:
      return 'D';
      break;
    case 14:
      return 'E';
      break;
    case 15:
      return 'F';
      break;
    default:
      perror("Problème dans la conversion en hex... \n");
      exit(EXIT_FAILURE);
      break;
  }
}

char *hexme(int entree){
  if (!entree){
    return "0";
  }
  int puissance = floor(log(entree)/log(16));
  char *hex = malloc(puissance * sizeof(char));
  int indice=0;
  while (entree >= 0 && puissance >= 0){
    // printf("entrée = %d, puissance = %d \n", entree, puissance);
    hex[indice] = calculer_coeff(&entree, puissance);
    puissance = puissance - 1;
    indice++;   
  }
  return hex;
}

int main(void){
  for (int entree = 14; entree < 300; entree++){
    printf("%d = %s \n",entree, hexme(entree));
  }
}