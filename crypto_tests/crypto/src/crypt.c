// FILE NAME  : crypt.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include "crypt.h"

//-----------------------------------------------------------------------------
// CRYPT
//-----------------------------------------------------------------------------
void
crypt(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output) {
  switch (type) {
    case 0       : xtea(key, input, enc_dec, output); break;
	  case 1 ... 3 : aes(key, input, type, enc_dec, output); break;
	  case 4 ... 7 : blowfish_128(key, input, type, enc_dec, output); break;
  }
}