#ifndef UART_H
#define UART_H

#ifndef __ASSEMBLER__

#include "base.h"
#include "mmio.h"
#include "gpio.h"

// UART-Funktionen
void uart_init();
void uart_writeText(char *buffer);
void uart_writeByteBlockingActual(unsigned char c);
char uart_readByteBlockingActual(void);
void putc(void* p, char c);

void debug_print(void);

#endif

#endif // UART_H
