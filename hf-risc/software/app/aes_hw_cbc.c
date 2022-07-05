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

#define BLOCK_SIZE 128
#define BUS_SIZE 32

const uint32_t aes_key[4] = {0xf0e1d2c3, 0xb4a59687, 0x78695a4b, 0x3c2d1e0f};
const uint8_t aes_tiny_key[16] = {0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b, 0x3c, 0x2d, 0x1e, 0x0f};

int main(void){
	uint8_t message[] = "Do not go gentle into that good";
	
	uint8_t iv[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	uint32_t iv_hw[4] = {0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF};
	uint32_t length, total_size, block_remaining, send_remaining, block_amount, send_amount, characters_to_add;
	uint32_t cycles_counter = 0;
	uint8_t flag_finished = 0;
	
	uint8_t msg[32] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};
	uint32_t msg_hw[8] = {0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF};
	uint32_t aes_return[4] = {0x00000000, 0x00000000,0x00000000,0x00000000};

	uint32_t cycles;

	length = sizeof(message);
	total_size = 8 * length;

	if(length == 0 || total_size < 128) {
		printf("Size smaller than 128 bits");
		return 0;
	}

	block_amount = (total_size) / 128;
	block_remaining = (total_size) % BLOCK_SIZE;
	characters_to_add = 16 - (block_remaining / 8);

	send_remaining = (total_size) % BUS_SIZE;
	send_amount = (total_size) / 32;

	if(block_remaining > 0) {
		block_amount++;
	}

	printf("\nL: %u | BR: %u | BA: %u | SR: %u | SA: %u", length, block_remaining, block_amount, send_remaining, send_amount);
	
	if(send_remaining > 0) {
		send_amount++;
	}

	uint8_t cbc_enc_output[length];

	printf("\nMessage:");
	hexdump((char *)message, sizeof(message));

	printf("\nL: %u | BR: %u | BA: %u | SR: %u | SA: %u", length, block_remaining, block_amount, send_remaining, send_amount);
	
	// CBC ENCRYPT
	
	cycles = TIMER0;
	AES128_CBC_encrypt_buffer(cbc_enc_output, message, length, aes_tiny_key, iv);
	cycles = TIMER0 - cycles;
	printf("\n%d cycles", cycles);
	
	printf("\nEncrypted:");
	hexdump((char *)cbc_enc_output, sizeof(cbc_enc_output));
	
	// TODO: DECRYPT TA CAGANDO O SEGUNDO BLOCO

	// CBC DECRYPT
	cycles = TIMER0;
	AES128_CBC_decrypt_buffer(cbc_enc_output, cbc_enc_output, length, aes_tiny_key, iv);
	cycles = TIMER0 - cycles;
	printf("\n%d cycles", cycles);
	
	printf("\nDecrypted:");
	hexdump((char *)cbc_enc_output, sizeof(cbc_enc_output));
	
	printf("\nL: %u | BR: %u | BA: %u | SR: %u | SA: %u", length, block_remaining, block_amount, send_remaining, send_amount);
	
	// uint8_t formatted_message[block_amount * BLOCK_SIZE];
	// uint8_t help_message[characters_to_add];

	// memcpy(formatted_message, message, total_size);
	// memcpy(formatted_message + total_size, help_message, ((block_amount*BLOCK_SIZE)-total_size));

	// printf("\nFormated Message:");
	// hexdump((char *)formatted_message, sizeof(formatted_message));
	
	// printf("\nMessage:");
	// hexdump((char *)msg_hw, sizeof(msg_hw));
	
	uint32_t aes_hw_enc_return[send_amount];

	// ENCRYPT
	cycles = TIMER0;
	AES_CONTROL = 0xe7000000;

	while(flag_finished != 1) {
		AES_KEY_32_BITS = aes_key[0];
		AES_KEY_32_BITS = aes_key[1];
		AES_KEY_32_BITS = aes_key[2];
		AES_KEY_32_BITS = aes_key[3];

		if(block_remaining == 0) {
			if(cycles_counter == 0) {
				// AES_INPUT_32_BITS = (message[0] | (message[1] << 8) | (message[2] << 16) | (message[3] << 24))^(iv_hw[0]);
				// AES_INPUT_32_BITS = (message[4] | (message[5] << 8) | (message[6] << 16) | (message[7] << 24))^(iv_hw[1]);
				// AES_INPUT_32_BITS = (message[8] | (message[9] << 8) | (message[10] << 16) | (message[11] << 24))^(iv_hw[2]);
				// AES_INPUT_32_BITS = (message[12] | (message[13] << 8) | (message[14] << 16) | (message[15] << 24))^(iv_hw[3]);
				
				AES_INPUT_32_BITS = msg_hw[0]^(iv_hw[0]);
				AES_INPUT_32_BITS = msg_hw[1]^(iv_hw[1]);
				AES_INPUT_32_BITS = msg_hw[2]^(iv_hw[2]);
				AES_INPUT_32_BITS = msg_hw[3]^(iv_hw[3]);

				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_enc_return[0] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[1] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[2] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[3] = AES_OUTPUT_32_BITS;

				cycles_counter++;
			} else if(cycles_counter < block_amount - 1) {
				// AES_INPUT_32_BITS = (message[16] | (message[17] << 8) | (message[18] << 16) | (message[19] << 24))^(aes_hw_enc_return[0]);
				// AES_INPUT_32_BITS = (message[20] | (message[21] << 8) | (message[22] << 16) | (message[23] << 24))^(aes_hw_enc_return[1]);
				// AES_INPUT_32_BITS = (message[24] | (message[25] << 8) | (message[26] << 16) | (message[27] << 24))^(aes_hw_enc_return[2]);
				// AES_INPUT_32_BITS = (message[28] | (message[29] << 8) | (message[30] << 16) | (message[31] << 24))^(aes_hw_enc_return[3]);
				AES_INPUT_32_BITS = msg_hw[0]^(aes_hw_enc_return[0]);
				AES_INPUT_32_BITS = msg_hw[1]^(aes_hw_enc_return[1]);
				AES_INPUT_32_BITS = msg_hw[2]^(aes_hw_enc_return[2]);
				AES_INPUT_32_BITS = msg_hw[3]^(aes_hw_enc_return[3]);

				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_enc_return[4] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[5] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[6] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[7] = AES_OUTPUT_32_BITS;
				
				cycles_counter++;
			} else {
				AES_INPUT_32_BITS = msg_hw[4]^(aes_hw_enc_return[0]);
				AES_INPUT_32_BITS = msg_hw[5]^(aes_hw_enc_return[1]);
				AES_INPUT_32_BITS = msg_hw[6]^(aes_hw_enc_return[2]);
				AES_INPUT_32_BITS = msg_hw[7]^(aes_hw_enc_return[3]);
				// AES_INPUT_32_BITS = (message[16] | (message[17] << 8) | (message[18] << 16) | (message[19] << 24))^(aes_hw_enc_return[0]);
				// AES_INPUT_32_BITS = (message[20] | (message[21] << 8) | (message[22] << 16) | (message[23] << 24))^(aes_hw_enc_return[1]);
				// AES_INPUT_32_BITS = (message[24] | (message[25] << 8) | (message[26] << 16) | (message[27] << 24))^(aes_hw_enc_return[2]);
				// AES_INPUT_32_BITS = (message[28] | (message[29] << 8) | (message[30] << 16) | (message[31] << 24))^(aes_hw_enc_return[3]);

				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_enc_return[4] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[5] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[6] = AES_OUTPUT_32_BITS;
				aes_hw_enc_return[7] = AES_OUTPUT_32_BITS;

				cycles = TIMER0 - cycles;

				cycles_counter = 0;
				flag_finished = 1;
			}
		}
	}
		
	printf("\n%d cycles", cycles);
	printf("\nenc message: %32x%32x%32x%32x%32x%32x%32x%32x", aes_hw_enc_return[0], aes_hw_enc_return[1], aes_hw_enc_return[2], aes_hw_enc_return[3], aes_hw_enc_return[4], aes_hw_enc_return[5], aes_hw_enc_return[6], aes_hw_enc_return[7]);
	// printf("\nEncrypted:");
	// hexdump((char *)aes_hw_enc_return, sizeof(aes_hw_enc_return));

	uint32_t aes_hw_dec_return[send_amount];

	cycles = TIMER0;
	AES_CONTROL = 0xe7000001;

	flag_finished = 0;

	while(flag_finished != 1) {
		AES_KEY_32_BITS = aes_key[0];
		AES_KEY_32_BITS = aes_key[1];
		AES_KEY_32_BITS = aes_key[2];
		AES_KEY_32_BITS = aes_key[3];

		if(block_remaining == 0) {
			// aes_data[ecb_counter-(16*cycles_counter)] = cbc_enc_output[cycles_counter][ecb_counter-(16*cycles_counter)];
			if(cycles_counter == 0) {
				AES_INPUT_32_BITS = aes_hw_enc_return[0];
				AES_INPUT_32_BITS = aes_hw_enc_return[1];
				AES_INPUT_32_BITS = aes_hw_enc_return[2];
				AES_INPUT_32_BITS = aes_hw_enc_return[3];
				
				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_dec_return[0] = (AES_OUTPUT_32_BITS)^(iv_hw[0]);
				aes_hw_dec_return[1] = (AES_OUTPUT_32_BITS)^(iv_hw[1]);
				aes_hw_dec_return[2] = (AES_OUTPUT_32_BITS)^(iv_hw[2]);
				aes_hw_dec_return[3] = (AES_OUTPUT_32_BITS)^(iv_hw[3]);

				cycles_counter++;
			} else if(cycles_counter < block_amount - 1) {
				AES_INPUT_32_BITS = aes_hw_enc_return[0];
				AES_INPUT_32_BITS = aes_hw_enc_return[1];
				AES_INPUT_32_BITS = aes_hw_enc_return[2];
				AES_INPUT_32_BITS = aes_hw_enc_return[3];
				
				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_enc_return[0 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(iv_hw[0]);
				aes_hw_enc_return[1 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(iv_hw[1]);
				aes_hw_enc_return[2 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(iv_hw[2]);
				aes_hw_enc_return[3 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(iv_hw[3]);

				cycles_counter++;
			} else {
				AES_INPUT_32_BITS = aes_hw_enc_return[4];
				AES_INPUT_32_BITS = aes_hw_enc_return[5];
				AES_INPUT_32_BITS = aes_hw_enc_return[6];
				AES_INPUT_32_BITS = aes_hw_enc_return[7];
				
				while (!(AES_CONTROL & AES_DONE));
				
				aes_hw_dec_return[0 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(aes_hw_enc_return[0]);
				aes_hw_dec_return[1 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(aes_hw_enc_return[1]);
				aes_hw_dec_return[2 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(aes_hw_enc_return[2]);
				aes_hw_dec_return[3 + (cycles_counter*4)] = (AES_OUTPUT_32_BITS)^(aes_hw_enc_return[3]);

				cycles = TIMER0 - cycles;

				cycles_counter = 0;
				flag_finished = 1;
			}
		}
	}

	printf("\n%d cycles", cycles);

	printf("\ndec message: %32x%32x%32x%32x%32x%32x%32x%32x", aes_hw_dec_return[0], aes_hw_dec_return[1], aes_hw_dec_return[2], aes_hw_dec_return[3], aes_hw_dec_return[4], aes_hw_dec_return[5], aes_hw_dec_return[6], aes_hw_dec_return[7]);
	
	return 0;
}
