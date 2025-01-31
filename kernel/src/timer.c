#include "mmio.h"
#include "timer.h"
#include "printf.h"
#include "irq.h"
#include "scheduler.h"
#include "pmu.h"

int timer_frq = 1000000;

void timer_init(void) {
    mmio_write(GICD_ENABLE + 4 * (PIT_SPI / 32), 1 << (PIT_SPI % 32));
    mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + timer_frq);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);
}

void timer_handler(void) {
    printf("t\n\r");
    mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + timer_frq);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);

    unsigned long count = 0;
    for (int i = 0; i < 5; i++) {
        count = read_pmu_counter(i);
        printf("%d: %u\n\r", i, count);
    }
    reset_pmu_counters();
    printf("\n\r");

    timer_tick();
}
