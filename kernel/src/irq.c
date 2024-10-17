#include "mmio.h"
#include "timer.h"
#include "irq.h"
#include "printf.h"

// Dispatch function for interrupts
void dispatch(void) {
    printf("ir_");
    unsigned int spi = mmio_read(GICC_ACK);
    while (spi != GIC_SPURIOUS) { // Loop until no SPIs are pending
        if (spi == PIT_SPI) {
            mmio_write(GICC_EOI, spi); // Signal the end of the interrupt
            spi = mmio_read(GICC_ACK); // Get the next interrupt
            timer_handler(); // Call the timer handler
        }
    }
}

// Initialize the GIC
void gic_init(void) {
    unsigned int i;

    mmio_write(GICD_CTLR, 0); // Disable distributor
    mmio_write(GICC_CTLR, 0); // Disable CPU interface

    // Disable, acknowledge, and deactivate all interrupts
    for (i = 0; i < (GIC_IRQS / 32); ++i) {
        mmio_write(GICD_DISABLE + 4 * i, ~0);
        mmio_write(GICD_PEND_CLR + 4 * i, ~0);
        mmio_write(GICD_ACTIVE_CLR + 4 * i, ~0);
    }

    // Route all interrupts to core 0
    for (i = 0; i < (GIC_IRQS / 4); ++i) {
        mmio_write(GICD_TARGET + 4 * i, 0x01010101);
        mmio_write(GICD_PRIO + 4 * i, 0xa0a0a0a0);
    }

    // Configure all interrupts as level-triggered
    for (i = 0; i < (GIC_IRQS / 16); ++i) {
        mmio_write(GICD_CFG + 4 * i, 0);
    }

    mmio_write(GICD_CTLR, 1); // Enable distributor
    mmio_write(GICC_PRIO, 0xF0); // Set interrupt priority threshold
    mmio_write(GICC_CTLR, 1); // Enable CPU interface
}

// Initialize interrupts
void interrupts_init(void) {
    disable_irq();
    gic_init(); // Initialize GIC
    enable_irq();
}

