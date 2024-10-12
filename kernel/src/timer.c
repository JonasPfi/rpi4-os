#include "mmio.h"
#include "timer.h"
#include "printf.h"


void timer_init(void) {
    mmio_write(GICD_ENABLE + 4 * (PIT_SPI / 32), 1 << (PIT_SPI % 32));
    mmio_write(PIT_Compare3, 12000000);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);
}

void timer_handler(void) {
    printf("t");
    mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + 5000000);
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);
}
