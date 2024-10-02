// io.h
#ifndef IO_H
#define IO_H

// UART Initialization
void uart_init(); // Initializes the UART peripheral

// UART Write
void uart_writeText(char *buffer); // Sends a string via UART
void uart_writeByteBlockingActual(unsigned char c); // Send a char via UART
void putc ( void* p, char c );

// UART Read
char uart_readByteBlockingActual(void); // Reads a char via UART

#endif // IO_H
