#ifndef GPIO_H
#define GPIO_H

#include "base.h"
#include "mmio.h"

// GPIO-Funktionen
void gpio_useAsAlt5(unsigned int pin_number);
unsigned int gpio_set(unsigned int pin_number, unsigned int value);
unsigned int gpio_clear(unsigned int pin_number, unsigned int value);
unsigned int gpio_pull(unsigned int pin_number, unsigned int value);
unsigned int gpio_function(unsigned int pin_number, unsigned int value);

#endif // GPIO_H
