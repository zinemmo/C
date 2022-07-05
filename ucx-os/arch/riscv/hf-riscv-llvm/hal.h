/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for HF-RISC
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

/* C type extensions */
typedef unsigned char			uint8_t;
typedef char				int8_t;
typedef unsigned short int		uint16_t;
typedef short int			int16_t;
typedef unsigned int			uint32_t;
typedef int				int32_t;
typedef unsigned long long		uint64_t;
typedef long long			int64_t;
typedef unsigned long			size_t;

extern uint32_t _stack_start;		/* Start of the STACK memory. */
extern uint32_t _stack_end;		/* End of the STACK memory. */
extern uint32_t _heap_start;		/* Start of the HEAP memory. */
extern uint32_t _heap_end;		/* End of the HEAP memory (one byte past the last byte of this memory). */
extern uint32_t _heap_size;		/* Size of HEAP memory. */
extern uint32_t _sidata;		/* Start address for the contents initialization of the .data
						section. defined in linker script. */
extern uint32_t _sdata;			/* Start address for the .data section, defined in linker script */
extern uint32_t _edata;			/* End address for the .data section, defined in linker script. */
extern uint32_t _sbss;			/* Start address for the .bss section, defined in linker script. */
extern uint32_t _ebss;			/* End address for the .bss section, defined in linker script. */
extern uint32_t _end;			/* Start address of the heap memory, defined in linker script. */

#define __ARCH__	"RV32I (HF-RISCV)"

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei(S)				_interrupt_set(S)

/* memory address map */
#define ROM_BASE			0x00000000
#define SYSTEM_BASE			0x20000000
#define RAM_BASE			0x40000000
#define EXTERNAL_BASE			0x60000000
#define EXTERNAL_UNCACHED_BASE		0xa0000000
#define S0_BASE				0xe1000000
#define S1_BASE				0xe2000000
#define S2_BASE				0xe4000000
#define S3_BASE				0xe8000000
#define INT_CONTROL_BASE		0xf0000000

#define EXT_SPI_24BA			0x30000000
#define EXT_SPI_16BA			0x31000000
#define EXT_SPI_24BA_UNCACHED		0x38000000
#define EXT_SPI_16BA_UNCACHED		0x39000000
#define EXT_SPI2_24BA			0x32000000
#define EXT_SPI2_16BA			0x33000000
#define EXT_SPI2_24BA_UNCACHED		0x3a000000
#define EXT_SPI2_16BA_UNCACHED		0x3b000000
#define EXT_SPI_WREN			0x39800000
#define EXT_SPI2_WREN			0x3b800000

#define EXT_SRAM			EXT_SPI_24BA
#define EXT_SRAM_UNCACHED		EXT_SPI_24BA_UNCACHED
#define EXT_EEPROM			EXT_SPI2_16BA
#define EXT_EEPROM_UNCACHED		EXT_SPI2_16BA_UNCACHED
#define EXT_EEPROM_WREN			EXT_SPI2_WREN


/* interrupt controller addresses and irq lines */
#define IRQ_VECTOR			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x000))
#define IRQ_CAUSE			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x010))
#define IRQ_MASK			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x020))
#define IRQ_STATUS			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x030))
#define IRQ_EPC				(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x040))
#define EXTIO_IN			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x080))
#define EXTIO_OUT			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x090))
#define DEBUG_ADDR			(*(volatile uint32_t *)(INT_CONTROL_BASE + 0x0d0))

#define MASK_IRQ0			(1 << 0)
#define MASK_IRQ1			(1 << 1)
#define MASK_IRQ2			(1 << 2)
#define MASK_IRQ3			(1 << 3)
#define MASK_IRQ4			(1 << 4)
#define MASK_IRQ5			(1 << 5)
#define MASK_IRQ6			(1 << 6)
#define MASK_IRQ7			(1 << 7)

/* peripheral addresses */

/* Segment 0 registers */
#define S0CAUSE				(*(volatile uint32_t *)(S0_BASE + 0x0400))
#define S0CAUSEINV			(*(volatile uint32_t *)(S0_BASE + 0x0800))
#define S0MASK				(*(volatile uint32_t *)(S0_BASE + 0x0c00))
#define PAALTCFG0			(*(volatile uint32_t *)(S0_BASE + 0x4000))
#define PBALTCFG0			(*(volatile uint32_t *)(S0_BASE + 0x4400))
#define PCALTCFG0			(*(volatile uint32_t *)(S0_BASE + 0x4800))
#define PDALTCFG0			(*(volatile uint32_t *)(S0_BASE + 0x4c00))
#define DEBUG0				(*(volatile uint32_t *)(S0_BASE + 0xd000))
#define DEBUG1				(*(volatile uint32_t *)(S0_BASE + 0xd400))
#define DEBUG2				(*(volatile uint32_t *)(S0_BASE + 0xd800))
#define DEBUG3				(*(volatile uint32_t *)(S0_BASE + 0xdc00))

#define MASK_S0CAUSE_BASE		(1 << 0)
#define MASK_S0CAUSE_GPIO		(1 << 1)
#define MASK_S0CAUSE_TIMER		(1 << 2)
#define MASK_S0CAUSE_UART		(1 << 3)
#define MASK_S0CAUSE_SPI		(1 << 4)
#define MASK_S0CAUSE_I2C		(1 << 5)

/* alternate pin configuration masks for PAALTCFG0 */
#define MASK_OSC0			0x00000001
#define MASK_PWM0			0x00000002
#define MASK_OSC1			0x00000004
#define MASK_PWM1			0x00000008
#define MASK_UART0			0x00000150
#define MASK_UART1			0x000002a0
#define MASK_SPI0			0x00001540
#define MASK_SPI1			0x0000003f
#define MASK_OSC0H			0x00010000
#define MASK_PWM0H			0x00020000
#define MASK_OSC1H			0x00040000
#define MASK_PWM1H			0x00080000
#define MASK_UART0H			0x01500000
#define MASK_UART1H			0x02a00000
#define MASK_SPI0H			0x15400000
#define MASK_SPI1H			0x003f0000

/* GPIO registers */
#define GPIO_BASE			(S0_BASE + 0x10000)
#define GPIOCAUSE			(*(volatile uint32_t *)(GPIO_BASE + 0x0400))
#define GPIOCAUSEINV			(*(volatile uint32_t *)(GPIO_BASE + 0x0800))
#define GPIOMASK			(*(volatile uint32_t *)(GPIO_BASE + 0x0c00))
#define PADDR				(*(volatile uint32_t *)(GPIO_BASE + 0x4000))
#define PAOUT				(*(volatile uint32_t *)(GPIO_BASE + 0x4010))
#define PAIN				(*(volatile uint32_t *)(GPIO_BASE + 0x4020))
#define PAININV				(*(volatile uint32_t *)(GPIO_BASE + 0x4030))
#define PAINMASK			(*(volatile uint32_t *)(GPIO_BASE + 0x4040))
#define PBDDR				(*(volatile uint32_t *)(GPIO_BASE + 0x4400))
#define PBOUT				(*(volatile uint32_t *)(GPIO_BASE + 0x4410))
#define PBIN				(*(volatile uint32_t *)(GPIO_BASE + 0x4420))
#define PBININV				(*(volatile uint32_t *)(GPIO_BASE + 0x4430))
#define PBINMASK			(*(volatile uint32_t *)(GPIO_BASE + 0x4440))
#define PCDDR				(*(volatile uint32_t *)(GPIO_BASE + 0x4800))
#define PCOUT				(*(volatile uint32_t *)(GPIO_BASE + 0x4810))
#define PCIN				(*(volatile uint32_t *)(GPIO_BASE + 0x4820))
#define PCININV				(*(volatile uint32_t *)(GPIO_BASE + 0x4830))
#define PCINMASK			(*(volatile uint32_t *)(GPIO_BASE + 0x4840))
#define PDDDR				(*(volatile uint32_t *)(GPIO_BASE + 0x4c00))
#define PDOUT				(*(volatile uint32_t *)(GPIO_BASE + 0x4c10))
#define PDIN				(*(volatile uint32_t *)(GPIO_BASE + 0x4c20))
#define PDININV				(*(volatile uint32_t *)(GPIO_BASE + 0x4c30))
#define PDINMASK			(*(volatile uint32_t *)(GPIO_BASE + 0x4c40))

#define MASK_PAIN			(1 << 0)
#define MASK_PBIN			(1 << 1)
#define MASK_PCIN			(1 << 2)
#define MASK_PDIN			(1 << 3)

#define MASK_P0				(1 << 0)
#define MASK_P1				(1 << 1)
#define MASK_P2				(1 << 2)
#define MASK_P3				(1 << 3)
#define MASK_P4				(1 << 4)
#define MASK_P5				(1 << 5)
#define MASK_P6				(1 << 6)
#define MASK_P7				(1 << 7)
#define MASK_P8				(1 << 8)
#define MASK_P9				(1 << 9)
#define MASK_P10			(1 << 10)
#define MASK_P11			(1 << 11)
#define MASK_P12			(1 << 12)
#define MASK_P13			(1 << 13)
#define MASK_P14			(1 << 14)
#define MASK_P15			(1 << 15)

/* Timer registers */
#define TIMER_BASE			(S0_BASE + 0x20000)
#define TIMERCAUSE			(*(volatile uint32_t *)(TIMER_BASE + 0x0400))
#define TIMERCAUSEINV			(*(volatile uint32_t *)(TIMER_BASE + 0x0800))
#define TIMERMASK			(*(volatile uint32_t *)(TIMER_BASE + 0x0c00))
#define TIMER0				(*(volatile uint32_t *)(TIMER_BASE + 0x4000))
#define TIMER1				(*(volatile uint32_t *)(TIMER_BASE + 0x4400))
#define TIMER1PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x4410))
#define TIMER1CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x4420))
#define TIMER1OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x4430))
#define TIMER2				(*(volatile uint32_t *)(TIMER_BASE + 0x4800))
#define TIMER2PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x4810))
#define TIMER2CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x4820))
#define TIMER2OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x4830))
#define TIMER3				(*(volatile uint32_t *)(TIMER_BASE + 0x4c00))
#define TIMER3PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x4c10))
#define TIMER3CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x4c20))
#define TIMER3OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x4c30))
#define TIMER4				(*(volatile uint32_t *)(TIMER_BASE + 0x5000))
#define TIMER4PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x5010))
#define TIMER4CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x5020))
#define TIMER4OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x5030))
#define TIMER5				(*(volatile uint32_t *)(TIMER_BASE + 0x5400))
#define TIMER5PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x5410))
#define TIMER5CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x5420))
#define TIMER5OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x5430))
#define TIMER6				(*(volatile uint32_t *)(TIMER_BASE + 0x5800))
#define TIMER6PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x5810))
#define TIMER6CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x5820))
#define TIMER6OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x5830))
#define TIMER7				(*(volatile uint32_t *)(TIMER_BASE + 0x5c00))
#define TIMER7PRE			(*(volatile uint32_t *)(TIMER_BASE + 0x5c10))
#define TIMER7CTC			(*(volatile uint32_t *)(TIMER_BASE + 0x5c20))
#define TIMER7OCR			(*(volatile uint32_t *)(TIMER_BASE + 0x5c30))

#define MASK_TIMER0A			(1 << 0)
#define MASK_TIMER0B			(1 << 1)
#define MASK_TIMER1CTC			(1 << 2)
#define MASK_TIMER1OCR			(1 << 3)
#define MASK_TIMER2CTC			(1 << 4)
#define MASK_TIMER2OCR			(1 << 5)
#define MASK_TIMER3CTC			(1 << 6)
#define MASK_TIMER3OCR			(1 << 7)
#define MASK_TIMER4CTC			(1 << 8)
#define MASK_TIMER4OCR			(1 << 9)
#define MASK_TIMER5CTC			(1 << 10)
#define MASK_TIMER5OCR			(1 << 11)
#define MASK_TIMER6CTC			(1 << 12)
#define MASK_TIMER6OCR			(1 << 13)
#define MASK_TIMER7CTC			(1 << 14)
#define MASK_TIMER7OCR			(1 << 15)

#define TIMERSET			(1 << 31)

#define TIMERPRE_DIV1			0x00
#define TIMERPRE_DIV4			0x01
#define TIMERPRE_DIV16			0x02
#define TIMERPRE_DIV64			0x03
#define TIMERPRE_DIV256			0x04
#define TIMERPRE_DIV1024		0x05
#define TIMERPRE_DIV4096		0x06
#define TIMERPRE_DIV16384		0x07

/* UART registers */
#define UART_BASE			(S0_BASE + 0x30000)
#define UARTCAUSE			(*(volatile uint32_t *)(UART_BASE + 0x0400))
#define UARTCAUSEINV			(*(volatile uint32_t *)(UART_BASE + 0x0800))
#define UARTMASK			(*(volatile uint32_t *)(UART_BASE + 0x0c00))
#define UART0				(*(volatile uint32_t *)(UART_BASE + 0x4000))
#define UART0DIV			(*(volatile uint32_t *)(UART_BASE + 0x4010))
#define UART1				(*(volatile uint32_t *)(UART_BASE + 0x4400))
#define UART1DIV			(*(volatile uint32_t *)(UART_BASE + 0x4410))
#define UART2				(*(volatile uint32_t *)(UART_BASE + 0x4800))
#define UART2DIV			(*(volatile uint32_t *)(UART_BASE + 0x4810))
#define UART3				(*(volatile uint32_t *)(UART_BASE + 0x4c00))
#define UART3DIV			(*(volatile uint32_t *)(UART_BASE + 0x4c10))
#define UART4				(*(volatile uint32_t *)(UART_BASE + 0x5000))
#define UART4DIV			(*(volatile uint32_t *)(UART_BASE + 0x5010))
#define UART5				(*(volatile uint32_t *)(UART_BASE + 0x5400))
#define UART5DIV			(*(volatile uint32_t *)(UART_BASE + 0x5410))
#define UART6				(*(volatile uint32_t *)(UART_BASE + 0x5800))
#define UART6DIV			(*(volatile uint32_t *)(UART_BASE + 0x5810))
#define UART7				(*(volatile uint32_t *)(UART_BASE + 0x5c00))
#define UART7DIV			(*(volatile uint32_t *)(UART_BASE + 0x5c10))

#define MASK_UART0_DATAAVAIL		(1 << 0)
#define MASK_UART0_WRITEBUSY		(1 << 1)
#define MASK_UART1_DATAAVAIL		(1 << 2)
#define MASK_UART1_WRITEBUSY		(1 << 3)
#define MASK_UART2_DATAAVAIL		(1 << 4)
#define MASK_UART2_WRITEBUSY		(1 << 5)
#define MASK_UART3_DATAAVAIL		(1 << 6)
#define MASK_UART3_WRITEBUSY		(1 << 7)
#define MASK_UART4_DATAAVAIL		(1 << 8)
#define MASK_UART4_WRITEBUSY		(1 << 9)
#define MASK_UART5_DATAAVAIL		(1 << 10)
#define MASK_UART5_WRITEBUSY		(1 << 11)
#define MASK_UART6_DATAAVAIL		(1 << 12)
#define MASK_UART6_WRITEBUSY		(1 << 13)
#define MASK_UART7_DATAAVAIL		(1 << 14)
#define MASK_UART7_WRITEBUSY		(1 << 15)

/* SPI registers */
#define SPI_BASE			(S0_BASE + 0x40000)
#define SPICAUSE			(*(volatile uint32_t *)(SPI_BASE + 0x0400))
#define SPICAUSEINV			(*(volatile uint32_t *)(SPI_BASE + 0x0800))
#define SPIMASK				(*(volatile uint32_t *)(SPI_BASE + 0x0c00))
#define SPI0				(*(volatile uint32_t *)(SPI_BASE + 0x4000))
#define SPI0CTRL			(*(volatile uint32_t *)(SPI_BASE + 0x4010))
#define SPI1				(*(volatile uint32_t *)(SPI_BASE + 0x4400))
#define SPI1CTRL			(*(volatile uint32_t *)(SPI_BASE + 0x4410))
#define SPI2				(*(volatile uint32_t *)(SPI_BASE + 0x4800))
#define SPI2CTRL			(*(volatile uint32_t *)(SPI_BASE + 0x4810))
#define SPI3				(*(volatile uint32_t *)(SPI_BASE + 0x4c00))
#define SPI3CTRL			(*(volatile uint32_t *)(SPI_BASE + 0x4c10))

#define MASK_SPI0_DATAVALID		(1 << 0)
#define MASK_SPI1_DATAVALID		(1 << 1)
#define MASK_SPI2_DATAVALID		(1 << 2)
#define MASK_SPI3_DATAVALID		(1 << 3)

#define SPI0_SSN			(2 << 8)
#define SPI0_CLK			(2 << 10)
#define SPI0_MOSI			(2 << 12)
#define SPI0_MISO			(2 << 14)
#define SPI0H_SSN			(2 << 24)
#define SPI0H_CLK			(2 << 26)
#define SPI0H_MOSI			(2 << 28)
#define SPI0H_MISO			(2 << 30)
#define SPI1_SSN			(3 << 0)
#define SPI1_CLK			(3 << 2)
#define SPI1_MOSI			(3 << 4)
#define SPI1_MISO			(3 << 6)
#define SPI1H_SSN			(3 << 16)
#define SPI1H_CLK			(3 << 18)
#define SPI1H_MOSI			(3 << 20)
#define SPI1H_MISO			(3 << 22)

#define SPI_DATA_XFER			(1 << 0)
#define SPI_DATA_VALID			(1 << 1)

#define SPI_CLK_DIV4			0x000
#define SPI_CLK_DIV8			0x008
#define SPI_CLK_DIV16			0x018
#define SPI_CLK_DIV32			0x038
#define SPI_CLK_DIV64			0x078
#define SPI_CLK_DIV128			0x0f8
#define SPI_CLK_DIV256			0x1f8
#define SPI_CLK_DIV512			0x3f8

/* I2C registers */
#define I2C_BASE			(S0_BASE + 0x50000)
#define I2CCAUSE			(*(volatile uint32_t *)(I2C_BASE + 0x0400))
#define I2CCAUSEINV			(*(volatile uint32_t *)(I2C_BASE + 0x0800))
#define I2CMASK				(*(volatile uint32_t *)(I2C_BASE + 0x0c00))
#define I2C0				(*(volatile uint32_t *)(I2C_BASE + 0x4000))
#define I2C1				(*(volatile uint32_t *)(I2C_BASE + 0x4400))
#define I2C2				(*(volatile uint32_t *)(I2C_BASE + 0x4800))
#define I2C3				(*(volatile uint32_t *)(I2C_BASE + 0x4c00))

/* DAC registers */
#define DAC_BASE			(S0_BASE + 0x60000)
#define DACCAUSE			(*(volatile uint32_t *)(DAC_BASE + 0x0400))
#define DACCAUSEINV			(*(volatile uint32_t *)(DAC_BASE + 0x0800))
#define DACMASK				(*(volatile uint32_t *)(DAC_BASE + 0x0c00))
#define DAC0				(*(volatile uint32_t *)(DAC_BASE + 0x4000))
#define DAC1				(*(volatile uint32_t *)(DAC_BASE + 0x4400))
#define DAC2				(*(volatile uint32_t *)(DAC_BASE + 0x4800))
#define DAC3				(*(volatile uint32_t *)(DAC_BASE + 0x4c00))

/* ADC registers */
#define ADC_BASE			(S0_BASE + 0x70000)
#define ADCCAUSE			(*(volatile uint32_t *)(ADC_BASE + 0x0400))
#define ADCCAUSEINV			(*(volatile uint32_t *)(ADC_BASE + 0x0800))
#define ADCMASK				(*(volatile uint32_t *)(ADC_BASE + 0x0c00))
#define ADC0				(*(volatile uint32_t *)(ADC_BASE + 0x4000))
#define ADC1				(*(volatile uint32_t *)(ADC_BASE + 0x4400))
#define ADC2				(*(volatile uint32_t *)(ADC_BASE + 0x4800))
#define ADC3				(*(volatile uint32_t *)(ADC_BASE + 0x4c00))

#define XTEA_BASE			0xfa000000
#define XTEA_CONTROL			(*(volatile uint32_t *)(XTEA_BASE + 0x000))
#define XTEA_KEY0			(*(volatile uint32_t *)(XTEA_BASE + 0x010))
#define XTEA_KEY1			(*(volatile uint32_t *)(XTEA_BASE + 0x020))
#define XTEA_KEY2			(*(volatile uint32_t *)(XTEA_BASE + 0x030))
#define XTEA_KEY3			(*(volatile uint32_t *)(XTEA_BASE + 0x040))
#define XTEA_IN0			(*(volatile uint32_t *)(XTEA_BASE + 0x050))
#define XTEA_IN1			(*(volatile uint32_t *)(XTEA_BASE + 0x060))
#define XTEA_OUT0			(*(volatile uint32_t *)(XTEA_BASE + 0x070))
#define XTEA_OUT1			(*(volatile uint32_t *)(XTEA_BASE + 0x080))

/* hardware dependent C library stuff */
typedef uint32_t jmp_buf[20];

int32_t _interrupt_set(int32_t s);
int32_t setjmp(jmp_buf env);
void longjmp(jmp_buf env, int32_t val);
extern int32_t _syscall(int32_t service, int32_t arg0, int32_t arg1, int32_t arg2);

void _putchar(char value);
int32_t _kbhit(void);
int32_t _getchar(void);

void _delay_ms(uint32_t msec);
void _delay_us(uint32_t usec);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);

#define strcpy(dst, src)		ucx_strcpy(dst, src)
#define strncpy(s1, s2, n)		ucx_strncpy(s1, s2, n)
#define strcat(dst, src)		ucx_strcat(dst, src)
#define strncat(dst, src, n)		ucx_strncat(dst, src, n)
#define strcmp(s1, s2)			ucx_strcmp(s1, s2)
#define strncmp(s1, s2, n)		ucx_strncmp(s1, s2, n)
#define strstr(string, find)		ucx_strstr(string, find)
#define strlen(s)			ucx_strlen(s)
#define strchr(s, c)			ucx_strchr(s, c)
#define strpbrk(str, set)		ucx_strpbrk(str, set)
#define strsep(pp, delim)		ucx_strsep(pp, delim)
#define strtok(s, delim)		ucx_strtok(s, delim)
#define strtol(s, end, base)		ucx_strtol(s, end, base)
#define memcpy(dst, src, n)		ucx_memcpy(dst, src, n)
#define memmove(dst, src, n)		ucx_memmove(dst, src, n)
#define memcmp(cs, ct, n)		ucx_memcmp(cs, ct, n)
#define memset(s, c, n)			ucx_memset(s, c, n)
#define abs(n)				ucx_abs(n)
#define random()			ucx_random()
#define srand(seed)			ucx_srand(seed)
#define puts(str)			ucx_puts(str)
#define gets(s)				ucx_gets(s)
#define getline(s)			ucx_getline(s)
#define printf(fmt, ...)		ucx_printf(fmt, ##__VA_ARGS__)
#define sprintf(out, fmt, ...)		ucx_sprintf(out, fmt, ##__VA_ARGS__)

#define malloc(n)			ucx_malloc(n)
#define free(n)				ucx_free(n)
#define calloc(n, t)			ucx_calloc(n, t) 
#define realloc(p, s)			ucx_realloc(p, s)

void krnl_dispatcher(void);

#define DEFAULT_GUARD_SIZE	2048
