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
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'A';
    case 11:
      return 'B';
    case 12:
      return 'C';
    case 13:
      return 'D';
    case 14:
      return 'E';
    case 15:
      return 'F';
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
    return "00";
  }
  if (entree < 0){
    entree = pow(256, 2) - abs(entree);
  }
  int puissance = floor(log(entree)/log(16));
  if (puissance%2 == 0){
    decal++;
  }
  char *hex = malloc((2*puissance * sizeof(char)));
  int indice=0;
  while (puissance >= 0){
    if (decal && indice==1){
      hex[0] = '0';
      indice++;
    } else {
      hex[indice] = calculer_coeff(&entree, puissance);
      puissance = puissance - 1;
      indice++;}
  }
  hex[indice]='\0';
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
