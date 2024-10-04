#include "mmio.h"

void mmio_write(long reg, unsigned int val) {
    *(volatile unsigned int *)reg = val;
}

unsigned int mmio_read(long reg) {
    return *(volatile unsigned int *)reg;
}

unsigned int get_el(void) {
    unsigned int el;
    asm volatile (
        "mrs %0, CurrentEL\n\t"
        "lsr %0, %0, #2\n\t"
        : "=r" (el)
    );
    return el;
}

