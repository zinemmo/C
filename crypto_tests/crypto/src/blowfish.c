// FILE NAME  : blowfish.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "blowfish.h"

//-----------------------------------------------------------------------------
// Blowfish: 64-bits
//-----------------------------------------------------------------------------
unsigned int
F(BLOWFISH_CTX *ctx, unsigned int x) {
  unsigned int a, b, c, d;
  unsigned int y;

  d = (unsigned int)(x & 0xFF);
  x >>= 8;
  c = (unsigned int)(x & 0xFF);
  x >>= 8;
  b = (unsigned int)(x & 0xFF);
  x >>= 8;
  a = (unsigned int)(x & 0xFF);
  y = ctx->S[0][a] + ctx->S[1][b];
  y = y ^ ctx->S[2][c];
  y = y + ctx->S[3][d];

  return y;
}

void
blowfish_enc(BLOWFISH_CTX *ctx, unsigned int *xl, unsigned int *xr){
  int   i;
  unsigned int Xl, Xr, temp;

  Xl = *xl;
  Xr = *xr;

  for (i = 0; i < N; ++i) {
    Xl = Xl ^ ctx->P[i];
    Xr = F(ctx, Xl) ^ Xr;

    temp = Xl;
    Xl = Xr;
    Xr = temp;
  }

  temp = Xl;
  Xl = Xr;
  Xr = temp;

  Xr = Xr ^ ctx->P[N];
  Xl = Xl ^ ctx->P[N + 1];

  *xl = Xl;
  *xr = Xr;
}

void
blowfish_dec(BLOWFISH_CTX *ctx, unsigned int *xl, unsigned int *xr){
  int   i;
  unsigned int Xl, Xr, temp;

  Xl = *xl;
  Xr = *xr;

  for (i = N + 1; i > 1; --i) {
    Xl = Xl ^ ctx->P[i];
    Xr = F(ctx, Xl) ^ Xr;

    temp = Xl;
    Xl = Xr;
    Xr = temp;
  }

  temp = Xl;
  Xl = Xr;
  Xr = temp;

  Xr = Xr ^ ctx->P[1];
  Xl = Xl ^ ctx->P[0];

  *xl = Xl;
  *xr = Xr;
}

void
blowfish_init(BLOWFISH_CTX *ctx, unsigned int *key, int keyLen) {
  int i, j;
  unsigned int data, datal, datar;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 256; j++)
      ctx->S[i][j] = ORIG_S[i][j];
  }

  j = 0;
  for (i = 0; i < N + 2; ++i) {
    data = 0x00000000;
    ctx->P[i] = ORIG_P[i] ^ key[i % keyLen];
  }

  datal = 0x00000000;
  datar = 0x00000000;

  for (i = 0; i < N + 2; i += 2) {
    blowfish_enc(ctx, &datal, &datar);
    ctx->P[i] = datal;
    ctx->P[i + 1] = datar;
  }

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 256; j += 2) {
      blowfish_enc(ctx, &datal, &datar);
      ctx->S[i][j] = datal;
      ctx->S[i][j + 1] = datar;
    }
  }
}

//-----------------------------------------------------------------------------
// Blowfish: 128-bits
//-----------------------------------------------------------------------------
void
blowfish_128(unsigned int* key, unsigned int* input, int type, int enc_dec, unsigned int *output) {
  int i;
  int keysize;
  unsigned int *L, *R;
  BLOWFISH_CTX ctx;

  keysize = 2*(type-2); // 4 to 128 bits, 6 to 192 bits, 8 to 256 bits

  blowfish_init (&ctx, key, keysize);

  L = (unsigned int *) malloc(sizeof(unsigned int));
  R = (unsigned int *) malloc(sizeof(unsigned int));
  for(i = 0; i < 2; i++) {
    *L = input[i*2];
    *R = input[i*2+1];

    if (enc_dec)
      blowfish_enc(&ctx, L, R);
    else
      blowfish_dec(&ctx, L, R);

    output[i*2]   = *L;
    output[i*2+1] = *R;
  }
  free(L);
  free(R);
}