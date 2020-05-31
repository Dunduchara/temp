#include "usart_io_base.h"
#include "usart.h"
#define _inline __attribute__ ((always_inline))

typedef struct _usart_io
{
	USART_TypeDef *usart;
	void (*flush_buffer)(struct _usart_io *);
	uint8_t buffer[BUFSIZE];
	uint16_t top;
} usart_io_t;

static void __reader_buffer_flush (usart_io_t *io);

static void __writer_buffer_flush (usart_io_t *io);

void init_usart_io (usart_io_t *io, USART_TypeDef *usart, enum io_type type)
{
	io->usart = usart;
	io->top   = 0;
	
	switch (type)
	{
		case IO_READER:
			io->flush_buffer = __reader_buffer_flush;
			break;
		case IO_WRITER:
			io->flush_buffer = __writer_buffer_flush;
		  break;
	}
}

static inline void _flush (usart_io_t *io) _inline;

void write_usart (usart_io_t *writer, uint8_t *buffer, uint16_t size)
{
	if (writer->top + size >= BUFSIZE)
	{
		_flush (writer);
		usart_transmit_bytes (writer->usart, buffer, size);
	}
	else
	{
		while (size--)
		{
			writer->buffer[writer->top++] = *buffer;
		
			if (*buffer == '\n')
				_flush (writer);
		
			++buffer;
		}
	}
}

static void _flush (usart_io_t *io)
{
	io->flush_buffer (io);
}
		
static void __reader_buffer_flush (usart_io_t *reader)
{
		reader->top = 0;
}

static void __writer_buffer_flush (usart_io_t *writer)
{
	usart_transmit_bytes (writer->usart, writer->buffer, writer->top);
	writer->top = 0;
}
