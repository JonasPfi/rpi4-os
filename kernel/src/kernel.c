#include "utils.h"
#include "kernel.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"
#include "irq.h"
#include "timer.h"
#include "scheduler.h"
#include "fork.h"

unsigned long _regs[38];

void process(char *array)
{
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
    interrupts_init();

    int el = get_el();
    printf("\nException level: %d \r\n", el);

    timer_start();

	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde");
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}

    // the main loop ---------------------------------
    while (1) {
        schedule();
    }
}
