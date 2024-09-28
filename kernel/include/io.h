// io.h
#ifndef IO_H
#define IO_H

// UART Initialization
void uart_init(); // Initializes the UART peripheral

// UART Write Text
void uart_writeText(char *buffer); // Sends a string via UART

#endif // IO_H
