#include "kernel.h"
#include "uart.h"
#include "printf.h"
#include "mmio.h"

// a properly aligned message buffer with: 9x4 byte long mes-
// sage setting feature PL011 UART Clock to 3MHz (and some TAGs and
// sizes of parameter in message)
volatile unsigned int __attribute__((aligned(16))) mbox[9] = { 36, 0, 0x38002, 12, 8, 2, 3000000, 0, 0 };

unsigned long _regs[38];


void dispatch (void) {
    // get one of the pending "Shared Peripheral Interrupt"
    unsigned int spi =  mmio_read(GICC_ACK);
    printf("i");
    
    while (spi != GIC_SPURIOUS) { // loop until no SPIs are pending on GIC
        if (spi == PIT_SPI) {
            printf("t");
            mmio_write(PIT_Compare3, mmio_read(PIT_LOW) + 5000000); // next in 5sec
            mmio_write(PIT_STATUS, 1 << PIT_MASKBIT); // clear IRQ in System Timer chip
        }
        // clear the pending
        mmio_write(GICC_EOI, spi);
        // get the next
        spi =  mmio_read(GICC_ACK);
    }
}


// initialize GIC-400
void gic_init (void) {
    unsigned int i;

    // disable Distributor and CPU interface
    mmio_write(GICD_CTLR, 0);
    mmio_write(GICC_CTLR, 0);

    // disable, acknowledge and deactivate all interrupts
    for (i = 0; i < (GIC_IRQS/32); ++i) {
        mmio_write(GICD_DISABLE     + 4*i, ~0);
        mmio_write(GICD_PEND_CLR    + 4*i, ~0);
        mmio_write(GICD_ACTIVE_CLR  + 4*i, ~0);
    }

    // direct all interrupts to core 0 (=01) with default priority a0
    for (i = 0; i < (GIC_IRQS/4); ++i) {
        mmio_write(GICD_TARGET + 4*i, 0x01010101);
        mmio_write(GICD_PRIO   + 4*i, 0xa0a0a0a0);
    }

    // config all interrupts to level triggered
    for (i = 0; i < (GIC_IRQS/16); ++i) {
        mmio_write(GICD_CFG + 4*i, 0);
    }

    // enable Distributor
    mmio_write(GICD_CTLR, 1);

    // set Core0 interrupts mask theshold prio to F0, to react on higher prio a0
    mmio_write(GICC_PRIO, 0xF0);
    // enable CPU interface
    mmio_write(GICC_CTLR, 1);
}

void main () {
    uart_init();
    init_printf(0, putc);

    int el = get_el();
    printf("Exception level: %d \r\n", el);

    // interrupts off/mask
    asm volatile( "msr daifset, #2" );

    gic_init();

    // PIT plugin (System Timer)
    mmio_write(GICD_ENABLE + 4 * (PIT_SPI / 32), 1 << (PIT_SPI % 32));
    mmio_write(PIT_Compare3, 12000000); // inital first IRQ in 12sec
    mmio_write(PIT_STATUS, 1 << PIT_MASKBIT);

    // IRQs on
    asm volatile( "msr daifclr, #2" ); 

    // the main loop ---------------------------------
    unsigned long long x=0;
    while (1) {
        if (x%500000 == 0) {
            // we are in the main loop
            printf("m");
        }
        x++;
    }
}
