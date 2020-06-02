#include "stm32f0xx.h"
#define _inline __attribute__ ((always_inline))

static inline void _inline _usart_transmit_byte (USART_TypeDef *, uint8_t byte);

void usart_transmit_bytes (USART_TypeDef *usart, uint8_t *buffer, uint16_t n_bytes)
{
	while (n_bytes--)
	{
		_usart_transmit_byte (usart, *buffer++);
	}
}

static void _inline _usart_transmit_byte (USART_TypeDef *usart, uint8_t byte)
{
	while (!(usart->ISR & USART_ISR_TC))
		;
	usart->TDR = byte;
}
