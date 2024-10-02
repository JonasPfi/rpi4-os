#include "io.h"

void main()
{
    uart_init();
    uart_writeText("Hello world!\n");
    while (1) {
        uart_writeByteBlockingActual(uart_readByteBlockingActual());
    }
}
