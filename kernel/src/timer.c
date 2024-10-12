#include "mmio.h"
#include "timer.h"
#include "printf.h"
#include "irq.h"

int timer_frq = 5000000;

void timer_init(void) {
    mmio_write(GICD_ENABLE + 4 * (PIT_SPI / 32), 1 << (PIT_SPI % 32));
    mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + timer_frq);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);
}

void timer_start(void) {
    timer_init();
    enable_irq();
    printf("Started Timer.\n\r");
}

void timer_handler(void) {
    printf("t");
    mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + timer_frq);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);
}
