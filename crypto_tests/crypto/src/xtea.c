// FILE NAME  : xtea.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include "xtea.h"

//-----------------------------------------------------------------------------
// XTEA: 128-bits
//-----------------------------------------------------------------------------
void
xtea_enc(unsigned int **dest, const unsigned int **v, const unsigned int **k) {
  int i;
  unsigned int y0 = *v[0], z0 = *v[1], y1 = *v[2], z1 = *v[3];
  unsigned int sum = 0, delta = 0x9E3779B9;

  for(i = 0; i < 32; i++) {
    y0  += ((z0 << 4 ^ z0 >> 5) + z0) ^ (sum + *k[sum & 3]);
    y1  += ((z1 << 4 ^ z1 >> 5) + z1) ^ (sum + *k[sum & 3]);
    sum += delta;
    z0  += ((y0 << 4 ^ y0 >> 5) + y0) ^ (sum + *k[sum>>11 & 3]);
    z1  += ((y1 << 4 ^ y1 >> 5) + y1) ^ (sum + *k[sum>>11 & 3]);
  }
  *dest[0]=y0; *dest[1]=z0; *dest[2]=y1; *dest[3]=z1;
}

void
xtea_dec(unsigned int **dest, const unsigned int **v, const unsigned int **k) {
  int i;
  unsigned int y0 = *v[0], z0 = *v[1], y1 = *v[2], z1 = *v[3];
  unsigned int sum = 0xC6EF3720, delta = 0x9E3779B9;
  for(i = 0; i < 32; i++) {
    z1  -= ((y1 << 4 ^ y1 >> 5) + y1) ^ (sum + *k[sum>>11 & 3]);
    z0  -= ((y0 << 4 ^ y0 >> 5) + y0) ^ (sum + *k[sum>>11 & 3]);
    sum -= delta;
    y1  -= ((z1 << 4 ^ z1 >> 5) + z1) ^ (sum + *k[sum & 3]);
    y0  -= ((z0 << 4 ^ z0 >> 5) + z0) ^ (sum + *k[sum & 3]);
  }
  *dest[0]=y0; *dest[1]=z0; *dest[2]=y1; *dest[3]=z1;
}

void
xtea(unsigned int* key, unsigned int* input, int enc_dec, unsigned int *output) {
  int i;
  unsigned int* d[4];
  const unsigned int* v[4];
  const unsigned int* k[4];

  for(i = 0; i < 4; i++) {
    d[i] = &output[3-i];
    v[i] = &input[3-i];
    k[i] = &key[3-i];
  }

  if (enc_dec)
    xtea_enc(d, v, k);
  else
    xtea_dec(d, v, k);
}