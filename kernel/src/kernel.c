#include "utils.h"
#include "kernel.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"
#include "irq.h"
#include "timer.h"

unsigned long _regs[38];

void process_1()
{
    char *array = "12345";
	while (1){
		for (int i = 0; i < 5; i++){
			uart_writeByteBlockingActual(array[i]);
			delay(500000);
		}
	}
}

void process_2()
{
    char *array = "abcde";
    while (1){
        for (int i = 0; i < 5; i++){
            uart_writeByteBlockingActual(array[i]);
            delay(500000);
        }
    }
}

void main () {
    uart_init();
    init_printf(0, putc);
    disable_irq();
    interrupts_init();

    int el = get_el();
    printf("\nException level: %d \r\n", el);

    timer_init();
    enable_irq();


    // the main loop ---------------------------------
    while (1) {
    }
}
