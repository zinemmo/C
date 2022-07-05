#include <hf-risc.h>

void porta_handler(void)
{
	printf("PAIN: %04x\n", PAIN);
}

void portb_handler(void)
{
	printf("PBIN: %04x\n", PBIN);
}

void timer0a_handler(void)
{
	printf("TIMER0A\n");
}

void timer0b_handler(void)
{
	printf("TIMER0B\n");
}

void timer1ctc_handler(void)
{
	printf("TIMER1CTC\n");
}

void timer1ocr_handler(void)
{
	printf("TIMER1OCR\n");
}

int main(void){
	int32_t val;
	
	TIMER1PRE = TIMERPRE_DIV4;

	/* unlock TIMER1 for reset */
	TIMER1 = TIMERSET;
	TIMER1 = 0;

	/* TIMER1 PWM duty cycle, 80% */
	TIMER1OCR = 8000;
	TIMER1CTC = 10000;

	/* enable interrupt masks for TIMER0 and TIMER1 CTC and OCR events */
	TIMERMASK |= (MASK_TIMER0A | MASK_TIMER0B | MASK_TIMER1CTC | MASK_TIMER1OCR);
	/* enable interrupt mask for PORTA and PORTB inputs */
	GPIOMASK |= MASK_PAIN | MASK_PBIN;
	/* enable PORTA input pin 3 mask */
	PAINMASK |= MASK_P3;
	/* enable PORTB input pins 10 and 2 mask */
	PBINMASK |= MASK_P10 | MASK_P2;
	/* configure alternate function for PORTA pin 0 output (TIMER1 output, PWM generation) */
	PAALTCFG0 |= MASK_PWM0;

	val = syscall(555, 7, 8, 9);
	printf(" [%d]", val);

	for(;;){
		printf(".");
//		delay_ms(1);
	}
}
