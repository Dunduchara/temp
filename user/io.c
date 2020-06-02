#include "usart_io.h"
#include <stdarg.h>

#define NBUFSIZE 16
#define END_OF_BYTE_STRING '\0'

#define _inline __attribute__ ((always_inline))


enum meta_type {UNDEF_META, INTEGER, FLOAT, STRING, PERCENT};

static inline enum meta_type _parse_meta (const uint8_t **meta_str) _inline;

static inline const uint8_t *_itoa (int32_t n) _inline;

static inline const uint8_t *_ftoa (double n) _inline;

static inline uint16_t _byte_strlen (const uint8_t *str) _inline;

void uprintf (usart_io_t *writer, const uint8_t *fmt, ...)
{
	void *tmp;
	const uint8_t *fmt_ptr = fmt;
	va_list va;
	
	va_start (va, fmt);
	
	while (*fmt_ptr != '\0')
	{
		if (*fmt_ptr == '%')
		{
			write_usart (writer, fmt, fmt_ptr - fmt);
			++fmt_ptr;
			switch (_parse_meta (&fmt_ptr))
			{
				case INTEGER:
					tmp = (void *) _itoa (va_arg (va, int));
					write_usart (writer, (uint8_t *) tmp, _byte_strlen (tmp));
					break;
				case FLOAT:
					tmp = (void *) _ftoa (va_arg (va, double));
					write_usart (writer, (uint8_t *) tmp, _byte_strlen (tmp));
					break;
				case STRING:
					tmp = (void *) va_arg (va, uint8_t *);
					write_usart (writer, (uint8_t *) tmp, _byte_strlen (tmp));
					break;
				case PERCENT:
					write_usart (writer, (const uint8_t *) "%", 1);
					break;
				default :
					break;
			}
			fmt = fmt_ptr;
		}
		else
		{
			++fmt_ptr;
		}
	}
	write_usart (writer, fmt, _byte_strlen (fmt));
	va_end (va);
}

static inline enum meta_type _parse_meta (const uint8_t **meta_str)
{
	while (**meta_str != '\0')
	{
		switch (*(*meta_str)++)
		{
			case 's':
				return STRING;
			case 'f':
				return FLOAT;
			case 'd':
				return INTEGER;
			case '%':
				return PERCENT;
			default:
				goto EXIT;
		}
	}
	
	EXIT :
	
	return UNDEF_META;
}

static inline uint16_t _byte_strlen (const uint8_t *str)
{
	const uint8_t *_str = str;
	
	while (*_str != END_OF_BYTE_STRING)
		++_str;
	
	return _str - str;
}

static uint8_t _str_number_buffer [NBUFSIZE] = {END_OF_BYTE_STRING};

static inline const uint8_t *_ftoa (double n)
{
	return (const uint8_t *) "go away!!!";
}

static inline const uint8_t *_itoa (int32_t n)
{
	uint8_t is_signed = n >> 31;
	
	if (is_signed)
		n = ~n + 1;
	
	uint8_t pos = NBUFSIZE - 1;
	do
	{
		--pos;
		_str_number_buffer[pos] = '0' + n % 10;
	}
	while ((n /= 10) != 0);
	
	if (is_signed)
		_str_number_buffer[--pos] = '-';
	
	return &_str_number_buffer[pos];
}
