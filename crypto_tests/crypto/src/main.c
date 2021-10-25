// FILE NAME  : main.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h> // para usar strtol()
#include <string.h>
#include "crypt.h"

//-----------------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------------
int
main(int argc, char *argv[]) {
  int i, type, enc_dec;
  unsigned int cipher[4];
  char crypt_name[14];

  printf("-- Crypt. Algorithms (START) --\n");

  // Write key
  unsigned int key[8] = {
    0xDEADBEEFL, 0x01234567L, 0x89ABCDEFL, 0xDEADBEEFL,
    0xDEADBEEFL, 0x01234567L, 0x89ABCDEFL, 0xDEADBEEFL
  };

  // Write type
  // 0: XTEA, 1: AES-128, 2: AES-192, 3: AES-256
  // 4: BLOWFISH-128, 5: BLOWFISH-192, 6: BLOWFISH-256
  type = strtol(argv[1], NULL, 10);

  switch (type)
  {
    case 0:
      strcpy(crypt_name, "XTEA");
      break;
    case 1:
      strcpy(crypt_name, "AES-128");
      break;
    case 2:
      strcpy(crypt_name, "AES-192");
      break;
    case 3:
      strcpy(crypt_name, "AES-256");
      break;
    case 4:
      strcpy(crypt_name, "BLOWFISH-256");
      break;
    case 5:
      strcpy(crypt_name, "BLOWFISH-256");
      break;
    case 6:
      strcpy(crypt_name, "BLOWFISH-256");
      break;
    default:
      break;
  }

  //---------------------------------------------------------------------------
  printf("\n-- START ENC %s--\n", crypt_name);

  // Write plantext
  unsigned int plan[4] = {0xA5A5A5A5L, 0x01234567L, 0xFEDCBA98L, 0x5A5A5A5AL};
  printf("%08X %08X %08X %08X (Plan)\n", plan[0], plan[1], plan[2], plan[3]);

  // Write enc_dec
  enc_dec = 1;

  // Call Crypt. Function
  crypt(key, plan, type, enc_dec, cipher);
  
  printf("%08X %08X %08X %08X (Cipher)\n", cipher[0], cipher[1], cipher[2], cipher[3]);

  //---------------------------------------------------------------------------
  printf("\n-- START DEC %s--\n", crypt_name);

  // Write plantext
  for (i=0; i<4; i++)
    plan[i] = cipher[i];
  printf("%08X %08X %08X %08X (Plan)\n", plan[0], plan[1], plan[2], plan[3]);

  // Write enc_dec
  enc_dec = 0;

  // Call Crypt. Function
  crypt(key, plan, type, enc_dec, cipher);
  
  printf("%08X %08X %08X %08X (Cipher)\n", cipher[0], cipher[1], cipher[2], cipher[3]);
  
  if ((cipher[0] != 0xA5A5A5A5) || (cipher[1] != 0x01234567) ||
	  (cipher[2] != 0xFEDCBA98) || (cipher[3] != 0x5A5A5A5A)) {
    printf("CRYPT: ERROR!!!\n"); 	  	
  }
  else {
  	printf("CRYPT: OK\n");
  }

  printf("\n-- Crypt. Algorithms (END) --\n"); 
  return 0;
}