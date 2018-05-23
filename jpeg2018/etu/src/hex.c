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

int decme(char *hex)
{
    int8_t val1 = hex[1];
    if (val1 > 64) {
      val1 -= 55;
    } else { val1 -= 48;}
    int8_t val2 = hex[2];
    if (val2 > 64) {
      val2 -= 55;
    } else { val2 -= 48;}
    return 16*val1+val2;
}

char *hexme(int entree){
  int decal = 0;
  if (!entree){
    return "/00";
  }
  int puissance = floor(log(entree)/log(16));
  if (puissance%2 == 0){
    decal++;
  }
  char *hex = malloc((2*puissance * sizeof(char)));
  int indice=0;
  while (puissance >= 0){
    // printf("entrée = %d, puissance = %d \n", entree, puissance);
    if (decal && indice==1){
      hex[1] = '0';
      indice++;
    }
    if (indice%3 == 0){
      hex[indice] = '/';
      indice++;
    } else {
      hex[indice] = calculer_coeff(&entree, puissance);
      puissance = puissance - 1;
      indice++;}
  }
  return hex;
}

char *binme_n(int entree, int magnetude){
  char *bin = malloc((magnetude)*sizeof(char));
  int indice = 1;
  while (indice < magnetude+1){
    if (entree - pow(2, magnetude - indice) >= 0){
        entree = entree - pow(2, magnetude - indice);
        bin[indice-1] = '1';
    } else { bin[indice-1] = '0';}
    indice++;
  }
  bin[indice-1] = '\0';
  return bin;
}
