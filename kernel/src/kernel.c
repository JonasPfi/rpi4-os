#include "kernel.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"
#include "irq.h"
#include "timer.h"

// a properly aligned message buffer with: 9x4 byte long mes-
// sage setting feature PL011 UART Clock to 3MHz (and some TAGs and
// sizes of parameter in message)
volatile unsigned int __attribute__((aligned(16))) mbox[9] = { 36, 0, 0x38002, 12, 8, 2, 3000000, 0, 0 };

unsigned long _regs[38];


void main () {
    uart_init();
    init_printf(0, putc);
    interrupts_init();

    int el = get_el();
    printf("Exception level: %d \r\n", el);

    timer_start();

    // the main loop ---------------------------------
    unsigned long long x=0;
    while (1) {
        if (x%500000 == 0) {
            // we are in the main loop
            printf("m");
        }
        x++;
    }
}
