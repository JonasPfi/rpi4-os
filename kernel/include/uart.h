#ifndef UART_H
#define UART_H

#include "base.h"
#include "mmio.h"
#include "gpio.h"

// UART-Funktionen
void uart_init();
void uart_writeText(char *buffer);
void uart_writeByteBlockingActual(unsigned char c);
char uart_readByteBlockingActual(void);
void putc(void* p, char c);

#endif // UART_H
