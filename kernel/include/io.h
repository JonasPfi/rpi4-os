// io.h
#ifndef IO_H
#define IO_H

// UART Initialization
void uart_init(); // Initializes the UART peripheral

// UART Write
void uart_writeText(char *buffer); // Sends a string via UART
void uart_writeByteBlockingActual(unsigned char ch); // Send a char via UART

// UART Read
char uart_readByteBlockingActual(void); // Reads a char via UART

#endif // IO_H
