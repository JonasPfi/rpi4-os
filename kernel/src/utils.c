#include "utils.h"

void delay(long count) {
    while (count > 0) {
        count--;  // Decrease the counter
    }
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
