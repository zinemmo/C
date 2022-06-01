#include <hf-risc.h>
// TODO: RUN AES TO SEE IF LOAD_I IS THE START
// TODO: RUN AES TO SEE IF DONE_O IS SET ON OUT
#define AES_BASE			0xe7000000
#define AES_CONTROL			(*(volatile uint8_t *)(AES_BASE + 0x000))
#define AES_KEY_8_BITS		(*(volatile uint8_t *)(AES_BASE + 0x010))
#define AES_INPUT_8_BITS	(*(volatile uint8_t *)(AES_BASE + 0x020))
#define AES_OUTPUT_8_BITS	(*(volatile uint8_t *)(AES_BASE + 0x030))

#define AES_START			(1 << 0)
#define AES_ENCRYPT			(1 << 1)
#define AES_DECRYPT			(0 << 1)
#define AES_DONE			(1 << 2)

const uint8_t aes_key[16] = {0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b, 0x3c, 0x2d, 0x1e, 0x0f};

void encipher(uint32_t num_rounds, uint32_t v[2], uint32_t const key[4]){
	uint32_t i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;

	for (i = 0; i < num_rounds; i++){
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
	}
	v[0] = v0; v[1] = v1;
}

void decipher(uint32_t num_rounds, uint32_t v[2], uint32_t const key[4]){
	uint32_t i;
	uint32_t v0 = v[0], v1 = v[1], delta = 0x9E3779B9, sum = delta * num_rounds;

	for (i = 0; i < num_rounds; i++){
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
	}
	v[0] = v0; v[1] = v1;
}

int main(void){
	uint8_t aes_data[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};
	uint8_t aes_return[16];
	uint32_t cycles;

	// printf("message: %8x%8x\n", msg[0], msg[1]);
	// cycles = TIMER0;
	// encipher(32, msg, xtea_key);
	// cycles = TIMER0 - cycles;
	// printf("encipher: %8x%8x, %d cycles\n", msg[0], msg[1], cycles);
	// cycles = TIMER0;
	// decipher(32, msg, xtea_key);
	// cycles = TIMER0 - cycles;
	// printf("decipher: %8x%8x, %d cycles\n", msg[0], msg[1], cycles);

	// printf("message: %8x%8x\n", msg[0], msg[1]);

	for(int i = 0; i < 16; i++) {
		AES_KEY_8_BITS = aes_key[i];
	}

	AES_CONTROL = AES_ENCRYPT;

	cycles = TIMER0;
	for(int i = 0; i < 16; i++) {
		AES_INPUT_8_BITS = aes_data[i];
	}

	AES_CONTROL |= AES_START;

	while (!(AES_CONTROL & AES_DONE));
	AES_CONTROL &= ~AES_START;
	cycles = TIMER0 - cycles;

	for(int i = 0; i < 16; i++) {
		aes_return[i] = AES_OUTPUT_8_BITS;
	}

	printf("encipher: %8x, %d cycles\n", aes_return[0], cycles);

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
