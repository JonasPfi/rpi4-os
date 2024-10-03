#include "uart.h"
#include "printf.h"
#include "mmio.h"

void main()
{
    uart_init();
    init_printf(0, putc);

    int el = get_el();
    printf("Exception level: %d \r\n", el);

    while (1) {
        uart_writeByteBlockingActual(uart_readByteBlockingActual());
    }
}
