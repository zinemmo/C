/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA328p
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdint.h>
#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define __ARCH__	"ATMEGA328p"

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei(S)				_interrupt_set(S)

char _interrupt_set(char s);

void _putchar(char value);
int32_t _kbhit(void);
int32_t _getchar(void);

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

#define DEFAULT_GUARD_SIZE	128
