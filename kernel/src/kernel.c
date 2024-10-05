#include "kernel.h"

// a properly aligned message buffer with: 9x4 byte long mes-
// sage setting feature PL011 UART Clock to 3MHz (and some TAGs and
// sizes of parameter in message)
volatile unsigned int __attribute__((aligned(16))) mbox[9] = { 36, 0, 0x38002, 12, 8, 2, 3000000, 0, 0 };

unsigned long _regs[38];

void uart_send (unsigned int c) {
    /* wait until we can send */
    do{asm volatile("nop");}while(GET32(UART3_FR)&0x20);
    /* write the character to the buffer */
    PUT32(UART3_DR, c);
}

void dispatch (void) {
    // get one of the pending "Shared Peripheral Interrupt"
    unsigned int spi = GET32(GICC_ACK);
    uart_send('i');
    
    while (spi != GIC_SPURIOUS) { // loop until no SPIs are pending on GIC
        if (spi == PIT_SPI) {
            uart_send('t');
            PUT32(PIT_Compare3, GET32(PIT_LOW) + 5000000); // next in 5sec
            PUT32(PIT_STATUS, 1 << PIT_MASKBIT); // clear IRQ in System Timer chip
        }
        // clear the pending
        PUT32(GICC_EOI, spi);
        // get the next
        spi = GET32(GICC_ACK);
    }
}

// initialize PL011 UART3 on GPIO4 and 5
void uart_init (void) {
    PUT32(UART3_CR, 0); // turn off UART3
    
    // set up clock for consistent divisor values
    unsigned int req = (((unsigned int)((unsigned long)&mbox)&~0xF) | 0x8);
    // wait until we can write to the mailbox
    do{asm volatile("nop");}while(GET32(MBOX_STATUS) & MBOX_FULL);
    // write the address of our message to the mailbox with channel identifier
    PUT32(MBOX_WRITE, req);
    // now wait for the response
    while (
        ( GET32(MBOX_STATUS) & MBOX_EMPTY ) || ( GET32(MBOX_READ) != req )
    ) {}

    // map UART3 to GPIO pins
    register unsigned int r = GET32(GPFSEL0);
    r &= ~((7<<12)|(7<<15));    // 000 to gpio4, gpio5
    r |= (3<<12)|(3<<15);       // 011 = alt4
    PUT32(GPFSEL0, r);

    // remove pullup or pulldown
    r = GET32(GPPUPPDN0);
    r &= ~((3<<8)|(3<<10));     // 11 to gpio4, gpio5 (mask/clear)
    r |= (0<<8)|(0<<10);        // 00 = no pullup or down
    PUT32(GPPUPPDN0, r);

    PUT32(UART3_ICR, 0x7FF);    // clear interrupts in PL011 Chip
    
    // Divider = 3000000MHz/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    PUT32(UART3_IBRD, 1);       // 115200 baud = 1 40
    PUT32(UART3_FBRD, 40);
    
    // Enable FIFO and 8 bit data transmission (1 stop bit, no parity)
    PUT32(UART3_LCRH, (1 << 6) | (1 << 5) | (1 << 4));
    // enable Tx, Rx, FIFO 
    PUT32(UART3_CR, (1 << 9) | (1 << 8) | (1 << 0));
}

// initialize GIC-400
void gic_init (void) {
    unsigned int i;
    
    // disable Distributor and CPU interface
    PUT32(GICD_CTLR, 0);
    PUT32(GICC_CTLR, 0);

    // disable, acknowledge and deactivate all interrupts
    for (i = 0; i < (GIC_IRQS/32); ++i) {
        PUT32(GICD_DISABLE     + 4*i, ~0);
        PUT32(GICD_PEND_CLR    + 4*i, ~0);
        PUT32(GICD_ACTIVE_CLR  + 4*i, ~0);
    }

    // direct all interrupts to core 0 (=01) with default priority a0
    for (i = 0; i < (GIC_IRQS/4); ++i) {
        PUT32(GICD_TARGET + 4*i, 0x01010101);
        PUT32(GICD_PRIO   + 4*i, 0xa0a0a0a0);
    }

    // config all interrupts to level triggered
    for (i = 0; i < (GIC_IRQS/16); ++i) {
        PUT32(GICD_CFG + 4*i, 0);
    }

    // enable Distributor
    PUT32(GICD_CTLR, 1);

    // set Core0 interrupts mask theshold prio to F0, to react on higher prio a0
    PUT32(GICC_PRIO, 0xF0);
    // enable CPU interface
    PUT32(GICC_CTLR, 1);
}

void main () {
    // interrupts off/mask
    asm volatile( "msr daifset, #2" );

    gic_init();

    // PIT plugin (System Timer)
    PUT32(GICD_ENABLE + 4 * (PIT_SPI / 32), 1 << (PIT_SPI % 32));
    PUT32(PIT_Compare3, 12000000); // inital first IRQ in 12sec
    PUT32(PIT_STATUS, 1 << PIT_MASKBIT);

    // IRQs on
    asm volatile( "msr daifclr, #2" ); 

    uart_init();
    
    // welcome from the uart !!!
    uart_send('b');
    uart_send('o');
    uart_send('o');
    uart_send('\r');
    uart_send('\n');
    
    // the main loop ---------------------------------
    unsigned long long x=0;
    while (1) {
        if (x%500000 == 0) {
            // we are in the main loop
            uart_send('m');
        }
        x++;
    }
}
