// ----------------------------------------------------------------
// ESPECIFICAÇÃO: O programa deve determinar se um identificador
// é ou não válido. Um identificador válido deve começar com uma
// letra e conter apenas letras ou dígitos. Além disto, deve ter
// no mínimo 1 caractere e no máximo 6 caracteres de comprimento
// ----------------------------------------------------------------
#include "identifier.h"
#include <stdio.h>

int valid_s(char ch) {
  if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')))
    return 1;
  else
    return 0;
}

int valid_f(char ch) {
  if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')))
    return 1;
  else
    return 0;
}

int identifier(char *ident, int prnt) {
  char achar;
  int  length, valid_id;
  length = 0;

  if(prnt)
    printf("Identificador: %s\n", ident);
  
  achar = ident[length]; // Pega 0 primeiro digito ou caracter
  valid_id = valid_s(achar); // Verifica se é digito, já que o primeiro não deve ser
  
  if(valid_id) { // Se não for digito adiciona um no tamanho, ou seja, fixa o caracter
    length = 1;
  }

  // Se for válido o primeiro dígito agora achar tem o segundo dígito
  // Se for inválido o primeiro dígito agora achar mantém com o primeiro dígito
  achar = ident[length]; 

  // OBSERVAÇÃO: A comparação do while deveria ser feita com \0 e não \n
  while(achar != '\0') { // Caso seja quebra de linha encerra o loop
    if(!(valid_f(achar))) { // Se o dígito ou caracter seja inválido entra no if
      valid_id = 0; // Caso algum dígito ou caracter seja inválido iguala valid_id a 0
    }
    length++; // Caso contrário adiciona uma posição no tamanho
    achar = ident[length]; // O novo digito ou caracter da mais nova posição
  }
  
  // OBSERVAÇÃO: A comparação deveria ser feita com (length <= 6) e não (length < 6)
  if (valid_id && (length >= 1) && (length < 6)) { // Confere se existe algum caracter ou dígito inválido e se o tamanho tem no mínimo 1 e no máximo 5
    if(prnt)
      printf("Valido\n");
    return 0;
  }
  
  else {
    if(prnt)
      printf("Invalido\n");
    return 1;
  }
}
