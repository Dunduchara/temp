#ifndef USART_H_
#define USART_H_

#include "stm32f0xx.h"

void usart_transmit_bytes (USART_TypeDef *usart, uint8_t *buffer, uint16_t n_bytes);

#endif // USART_H_
