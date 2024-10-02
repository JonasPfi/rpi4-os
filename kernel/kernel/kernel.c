#include "uart.h"
#include "printf.h"

void main()
{
    uart_init();
    init_printf(0, putc);
    printf("Hello from printf! \r\n Now using modular structure! \r\n");
    while (1) {
        uart_writeByteBlockingActual(uart_readByteBlockingActual());
    }
}
