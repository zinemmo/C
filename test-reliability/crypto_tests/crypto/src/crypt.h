// FILE NAME  : crypt.h
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include <stdint.h>
#include "xtea.h"
#include "aes.h"
#include "blowfish.h"

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void crypt(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output);
