// FILE NAME  : xtea.h
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

void xtea_enc(unsigned int **dest, const unsigned int **v, const unsigned int **k);
void xtea_dec(unsigned int **dest, const unsigned int **v, const unsigned int **k);
void xtea(unsigned int* key, unsigned int* input, int enc_dec, unsigned int *output);