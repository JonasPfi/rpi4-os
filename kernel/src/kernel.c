#include "utils.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"
#include "irq.h"
#include "timer.h"
#include "scheduler.h"
#include "fork.h"
#include "pmu.h"

void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_writeByteBlockingActual(array[i]);
			delay(100000);
		}
	}
}


void main () {
    uart_init();
    init_printf(0, putc);

    int el = get_el();
    printf("\n\rException level: %d \r\n", el);


    disable_irq();
    interrupts_init();
    timer_init();
    enable_irq();

    init_pmu();
    unsigned long current_count;
    current_count = read_instruction_count();
    printf("Current instructions counted: %d\n\r", current_count);
    reset_instruction_count();

	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}

    current_count = read_instruction_count();
    printf("Current instructions counted: %d\n\r", current_count);
    reset_instruction_count();

    current_count = read_instruction_count();
    printf("Current instructions counted: %d\n\r", current_count);
    reset_instruction_count();

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
