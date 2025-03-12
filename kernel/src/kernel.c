#include "utils.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"
#include "irq.h"
#include "timer.h"
#include "scheduler.h"
#include "fork.h"
#include "pmu.h"

#define ARRAY_SIZE 100000
volatile int array[ARRAY_SIZE];
static unsigned long int next = 1;

int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void process_complex_calculations() {
    int x = 1000;

    while (1) {
        x = (x * x) / 1000 + (2 * x) + 1000;
        x = 1000 / (x + 1000);
        x = (x * x) / 1000 - x;
        x = x + (x * x) / 2000;

        if (x > 1000000000) {
            x = 1000;
        } else if (x < -1000000000) {
            x = -1000;
        }
    }
}

void process_memory_accesses() {
    while (1) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = i;
        }
    }
}

void process_nothing() {
    while (1) {}
}

void process_idle() {
    while (1) {
        asm volatile("wfi");
    }
}

void process_instructions() {
    register int a = 0;
    register int b = 1;

    while (1) {
        a = a + b;
        b = a - b;

        __asm__ __volatile__ ("");
    }
}

void process_branch_mispredictions() {
    volatile int x = 0;
    while (1) {
        if (rand() % 2 == 0) {
            x = x + 1;
        } else {
            x = x - 1;
        }
    }
}


void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
//			uart_writeByteBlockingActual(array[i]);
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
    enable_pmu_counters();

    int res = copy_process((unsigned long)&process_memory_accesses, (unsigned long)"");
    if (res != 0) {
        printf("error while starting process 1");
        return;
    }

	res = copy_process((unsigned long)&process_idle, (unsigned long)"");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}

    res = copy_process((unsigned long)&process_memory_accesses, (unsigned long)"");
    if (res != 0) {
        printf("error while starting process 1");
        return;
    }

    res = copy_process((unsigned long)&process_branch_mispredictions, (unsigned long)"");
    if (res != 0) {
        printf("error while starting process 1");
        return;
    }

    // the main loop ---------------------------------
    while (1) {
       schedule();
    }
}
