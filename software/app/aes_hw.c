#include <hf-risc.h>
#include "aes_tiny/aes.h"

#define AES_BASE			0xe7000000
#define AES_CONTROL			(*(volatile uint32_t *)(AES_BASE + 0x000))
#define AES_KEY_32_BITS		(*(volatile uint32_t *)(AES_BASE + 0x010))
#define AES_INPUT_32_BITS	(*(volatile uint32_t *)(AES_BASE + 0x020))
#define AES_OUTPUT_32_BITS	(*(volatile uint32_t *)(AES_BASE + 0x030))

#define AES_DECRYPT			(1 << 0) // X1X
#define AES_ENCRYPT			(0 << 0) // X0X
#define AES_DONE			(1 << 1) // 1XX

const uint32_t aes_key[4] = {0xf0e1d2c3, 0xb4a59687, 0x78695a4b, 0x3c2d1e0f};
const uint8_t aes_tiny_key[16] = {0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b, 0x3c, 0x2d, 0x1e, 0x0f};

int main(void){
	uint8_t aes_data[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};
	uint32_t msg[4] = {0x12345678, 0x90123456, 0x12345678, 0x90123456};
	uint32_t aes_return[4] = {0x00000000, 0x00000000,0x00000000,0x00000000};
	uint8_t aes_tiny_enc_return[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t aes_tiny_dec_return[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	uint32_t cycles;

	printf("message: %8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x\n", aes_data[0], aes_data[1], aes_data[2], aes_data[3], aes_data[4], aes_data[5], aes_data[6], aes_data[7], aes_data[8], aes_data[9], aes_data[10], aes_data[11], aes_data[12], aes_data[13], aes_data[14], aes_data[15]);
	cycles = TIMER0;
	AES128_ECB_encrypt(&aes_data, &aes_tiny_key, &aes_tiny_enc_return);
	cycles = TIMER0 - cycles;
	printf("enc message: %8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x\n", aes_tiny_enc_return[0], aes_tiny_enc_return[1], aes_tiny_enc_return[2], aes_tiny_enc_return[3], aes_tiny_enc_return[4], aes_tiny_enc_return[5], aes_tiny_enc_return[6], aes_tiny_enc_return[7], aes_tiny_enc_return[8], aes_tiny_enc_return[9], aes_tiny_enc_return[10], aes_tiny_enc_return[11], aes_tiny_enc_return[12], aes_tiny_enc_return[13], aes_tiny_enc_return[14], aes_tiny_enc_return[15]);
	printf("%d cycles\n", cycles);
	cycles = TIMER0;
	AES128_ECB_decrypt(&aes_tiny_enc_return, &aes_tiny_key, &aes_tiny_dec_return);
	cycles = TIMER0 - cycles;
	printf("dec message: %8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x%8x\n", aes_tiny_dec_return[0], aes_tiny_dec_return[1], aes_tiny_dec_return[2], aes_tiny_dec_return[3], aes_tiny_dec_return[4], aes_tiny_dec_return[5], aes_tiny_dec_return[6], aes_tiny_dec_return[7], aes_tiny_dec_return[8], aes_tiny_dec_return[9], aes_tiny_dec_return[10], aes_tiny_dec_return[11], aes_tiny_dec_return[12], aes_tiny_dec_return[13], aes_tiny_dec_return[14], aes_tiny_dec_return[15]);
	printf("%d cycles\n", cycles);

	// ENCRYPT
	cycles = TIMER0;
	AES_CONTROL = 0xe7000000;

	AES_KEY_32_BITS = aes_key[0];
	AES_KEY_32_BITS = aes_key[1];
	AES_KEY_32_BITS = aes_key[2];
	AES_KEY_32_BITS = aes_key[3];

	AES_INPUT_32_BITS = msg[0];
	AES_INPUT_32_BITS = msg[1];
	AES_INPUT_32_BITS = msg[2];
	AES_INPUT_32_BITS = msg[3];

	while (!(AES_CONTROL & AES_DONE));

	aes_return[0] = AES_OUTPUT_32_BITS;
	aes_return[1] = AES_OUTPUT_32_BITS;
	aes_return[2] = AES_OUTPUT_32_BITS;
	aes_return[3] = AES_OUTPUT_32_BITS;
	cycles = TIMER0 - cycles;
	printf("%d cycles\n", cycles);

	// DECRYPT
	cycles = TIMER0;
	AES_CONTROL = 0xe7000001;

	AES_KEY_32_BITS = aes_key[0];
	AES_KEY_32_BITS = aes_key[1];
	AES_KEY_32_BITS = aes_key[2];
	AES_KEY_32_BITS = aes_key[3];

	AES_INPUT_32_BITS = aes_return[0];
	AES_INPUT_32_BITS = aes_return[1];
	AES_INPUT_32_BITS = aes_return[2];
	AES_INPUT_32_BITS = aes_return[3];

	while (!(AES_CONTROL & AES_DONE));

	aes_return[0] = AES_OUTPUT_32_BITS;
	aes_return[1] = AES_OUTPUT_32_BITS;
	aes_return[2] = AES_OUTPUT_32_BITS;
	aes_return[3] = AES_OUTPUT_32_BITS;

	cycles = TIMER0 - cycles;
	printf("%d cycles\n", cycles);

	return 0;
}
