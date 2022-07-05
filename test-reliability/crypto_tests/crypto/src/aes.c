// FILE NAME  : aes.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2021-06-08   Initial version.
//-----------------------------------------------------------------------------

#include "aes.h"

//-----------------------------------------------------------------------------
// AES (128/192/256)
//-----------------------------------------------------------------------------
uint8_t
getSBoxValue(uint8_t num) {
  return sbox[num];
}

uint8_t
getSBoxInvert(uint8_t num) {
  return rsbox[num];
}

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
void
KeyExpansion(void) {
  uint32_t i, k;
  uint8_t tempa[4]; // Used for the column/row operations

  // The first round key is the key itself.
  for(i = 0; i < Nk; ++i) {
    RoundKey[(i * 4) + 0] = ((Key[i] >> 24) & 0xFF);
    RoundKey[(i * 4) + 1] = ((Key[i] >> 16) & 0xFF);
    RoundKey[(i * 4) + 2] = ((Key[i] >>  8) & 0xFF);
    RoundKey[(i * 4) + 3] =  (Key[i]        & 0xFF);
  }

  // All other round keys are found from the previous round keys.
  //i == Nk
  for(; i < Nb * (Nr + 1); ++i) {
    tempa[0] = RoundKey[(i-1) * 4 + 0];
    tempa[1] = RoundKey[(i-1) * 4 + 1];
    tempa[2] = RoundKey[(i-1) * 4 + 2];
    tempa[3] = RoundKey[(i-1) * 4 + 3];

    if (i % Nk == 0) {
    // This function shifts the 4 bytes in a word to the left once.
    // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      k = tempa[0];
      tempa[0] = tempa[1];
      tempa[1] = tempa[2];
      tempa[2] = tempa[3];
      tempa[3] = k;

      // SubWord() is a function that takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      tempa[0] = getSBoxValue(tempa[0]);
      tempa[1] = getSBoxValue(tempa[1]);
      tempa[2] = getSBoxValue(tempa[2]);
      tempa[3] = getSBoxValue(tempa[3]);

      tempa[0] = tempa[0] ^ Rcon[i/Nk];
    }

    if (Nk == 8) {
      if (i % Nk == 4) {
        // Function Subword()
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }
    }
    RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
void
AddRoundKey(uint8_t round) {
  uint8_t i, j;
  for(i = 0; i < 4; ++i) {
    for(j = 0; j < 4; ++j) {
      state[i][j] ^= RoundKey[round * Nb * 4 + i * Nb + j];
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void
SubBytes(void) {
  uint8_t i, j;
  for(i = 0; i < 4; ++i) {
    for(j = 0; j < 4; ++j) {
      state[j][i] = getSBoxValue(state[j][i]);
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void
ShiftRows(void) {
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp        = state[0][1];
  state[0][1] = state[1][1];
  state[1][1] = state[2][1];
  state[2][1] = state[3][1];
  state[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp        = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;

  temp        = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // Rotate third row 3 columns to left
  temp        = state[0][3];
  state[0][3] = state[3][3];
  state[3][3] = state[2][3];
  state[2][3] = state[1][3];
  state[1][3] = temp;
}

uint8_t
xtime(uint8_t x) {
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
void
MixColumns(void) {
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for(i = 0; i < 4; ++i) {  
    t   = state[i][0];
    Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3] ;
    Tm  = state[i][0] ^ state[i][1] ; Tm = xtime(Tm);  state[i][0] ^= Tm ^ Tmp ;
    Tm  = state[i][1] ^ state[i][2] ; Tm = xtime(Tm);  state[i][1] ^= Tm ^ Tmp ;
    Tm  = state[i][2] ^ state[i][3] ; Tm = xtime(Tm);  state[i][2] ^= Tm ^ Tmp ;
    Tm  = state[i][3] ^ t ;           Tm = xtime(Tm);  state[i][3] ^= Tm ^ Tmp ;
  }
}

// Multiply is used to multiply numbers in the field GF(2^8)
uint8_t
Multiply(uint8_t x, uint8_t y) {
  return (((y & 1) * x) ^
         ((y>>1 & 1) * xtime(x)) ^
         ((y>>2 & 1) * xtime(xtime(x))) ^
         ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
         ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
void
InvMixColumns(void) {
  uint8_t i;
  uint8_t a, b, c, d;
  for(i = 0; i < 4; ++i) { 
    a = state[i][0];
    b = state[i][1];
    c = state[i][2];
    d = state[i][3];

    state[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    state[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    state[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    state[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void
InvSubBytes(void) {
  uint8_t i, j;
  for(i = 0; i < 4; ++i) {
    for(j = 0; j < 4; ++j) {
      state[j][i] = getSBoxInvert(state[j][i]);
    }
  }
}

void
InvShiftRows(void) {
  uint8_t temp;

  // Rotate first row 1 columns to right  
  temp = state[3][1];
  state[3][1] = state[2][1];
  state[2][1] = state[1][1];
  state[1][1] = state[0][1];
  state[0][1] = temp;

  // Rotate second row 2 columns to right 
  temp = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;

  temp = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = state[0][3];
  state[0][3] = state[1][3];
  state[1][3] = state[2][3];
  state[2][3] = state[3][3];
  state[3][3] = temp;
}

// AES encoder
void
aes_enc(void) {
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(0); 

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for(round = 1; round < Nr; ++round) {
    SubBytes();
    ShiftRows();
    MixColumns();
    AddRoundKey(round);
  }

  // The last round is given below.
  // The MixColumns function is not here in the last round.
  SubBytes();
  ShiftRows();
  AddRoundKey(Nr);
}

// AES decoder
void
aes_dec(void) {
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(Nr); 

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for(round = Nr-1; round > 0; round--) {
    InvShiftRows();
    InvSubBytes();
    AddRoundKey(round);
    InvMixColumns();
  }

  // The last round is given below.
  // The MixColumns function is not here in the last round.
  InvShiftRows();
  InvSubBytes();
  AddRoundKey(0);
}

// AES: ECB mode
void
aes(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output) {
  uint8_t i;

  // Copy input to state
  for(i = 0; i < 4; i++) {
    state[i][0] = ((input[i] >> 24) & 0xFF);
    state[i][1] = ((input[i] >> 16) & 0xFF);
    state[i][2] = ((input[i] >>  8) & 0xFF);
    state[i][3] =  (input[i]        & 0xFF);
  }

  // Update some private variables
  switch (type) {
    case 3  : Nk = 8; KEYLEN = 32; Nr = 14; break; // 256 bits
    case 2  : Nk = 6; KEYLEN = 24; Nr = 12; break; // 192 bits
    default : Nk = 4; KEYLEN = 16; Nr = 10; break; // 128 bits
  }

  // The KeyExpansion routine must be called before encryption
  Key = key;
  KeyExpansion();

  // The next function call encrypts the PlainText with the Key using AES algorithm
  if (enc_dec)
    aes_enc();
  else
    aes_dec();

  // Copy state to output
  for(i = 0; i < 4; i++) {
    output[i] = (state[i][0] << 24) + (state[i][1] << 16) + (state[i][2] << 8) + state[i][3];
  }
}