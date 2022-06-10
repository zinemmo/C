#include <hf-risc.h>
// TODO: LOAD_I 1 when sending key and input to AES
// TODO: Done 1 when AES stopped 
#define AES_BASE			0xe7000000
#define AES_CONTROL			(*(volatile uint32_t *)(AES_BASE + 0x000))
#define AES_KEY_32_BITS		(*(volatile uint32_t *)(AES_BASE + 0x010))
#define AES_INPUT_32_BITS	(*(volatile uint32_t *)(AES_BASE + 0x020))
#define AES_OUTPUT_8_BITS	(*(volatile uint8_t *)(AES_BASE + 0x030))

#define AES_START			(1 << 0) // XX1
#define AES_DECRYPT			(1 << 1) // X1X
#define AES_ENCRYPT			(0 << 1) // X0X
#define AES_DONE			(1 << 2) // 1XX

const uint8_t aes_key[16] = {0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b, 0x3c, 0x2d, 0x1e, 0x0f};
const uint32_t xtea_key[4] = {0xf0e1d2c3, 0xb4a59687, 0x78695a4b, 0x3c2d1e0f};

int main(void){
	uint8_t aes_data[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};
	uint32_t msg[4] = {0x12345678, 0x90123456, 0x12345678, 0x90123456};
	uint8_t aes_return[16];

	AES_CONTROL |= AES_ENCRYPT;
	AES_CONTROL |= AES_START;

	AES_KEY_32_BITS = xtea_key[0];
	AES_KEY_32_BITS = xtea_key[1];
	AES_KEY_32_BITS = xtea_key[2];
	AES_KEY_32_BITS = xtea_key[3];

	AES_INPUT_32_BITS = msg[0];
	AES_INPUT_32_BITS = msg[1];
	AES_INPUT_32_BITS = msg[2];
	AES_INPUT_32_BITS = msg[3];

	while (!(AES_CONTROL & AES_DONE));
	AES_CONTROL &= ~AES_START;

	for(int i = 0; i < 16; i++) {
		aes_return[i] = AES_OUTPUT_8_BITS;
	}

	printf("encipher: %8x\n", aes_return[0]);

	// XTEA_CONTROL = AES_DECRYPT;

	// cycles = TIMER0;
	// XTEA_IN0 = msg[0];
	// XTEA_IN1 = msg[1];
	// XTEA_CONTROL = AES_START;
	// while (!(XTEA_CONTROL & 0x4));
	// XTEA_CONTROL &= ~AES_START;
	// cycles = TIMER0 - cycles;

	// msg[0] = XTEA_OUT0;
	// msg[1] = XTEA_OUT1;

	// printf("decipher: %8x%8x, %d cycles\n", msg[0], msg[1], cycles);

	return 0;
}
