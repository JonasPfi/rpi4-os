#include "gpio.h"

enum {
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4,
    GPIO_MAX_PIN    = 53,
    GPIO_FUNCTION_ALT5 = 2,
    Pull_None = 0,
};

static unsigned int gpio_call(unsigned int pin_number, unsigned int value, unsigned int base, unsigned int field_size, unsigned int field_max) {
    unsigned int field_mask = (1 << field_size) - 1;

    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0; 

    unsigned int num_fields = 32 / field_size;
    unsigned int reg = base + ((pin_number / num_fields) * 4);
    unsigned int shift = (pin_number % num_fields) * field_size;

    unsigned int curval = mmio_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}

unsigned int gpio_set(unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_clear(unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_pull(unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}

unsigned int gpio_function(unsigned int pin_number, unsigned int value) {
    return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}

void gpio_useAsAlt5(unsigned int pin_number) {
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}
