#ifndef USART_H_
#define USART_H_

#include "usart_io_base.h"

typedef struct _usart_io
{
	uint8_t __mem[2 * sizeof (void *) + 2 + BUFSIZE];
} usart_io_t;

void init_usart_io (usart_io_t *io, USART_TypeDef *usart, enum io_type);

void write_usart (usart_io_t *io, const uint8_t *buffer, uint16_t size);

#endif // USART_H_
