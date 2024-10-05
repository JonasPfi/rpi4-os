#include "uart.h"

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK  = 500000000,
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init() {
    mmio_write(AUX_ENABLES, 1); // enable UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
    gpio_useAsAlt5(14);
    gpio_useAsAlt5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

static unsigned int uart_isWriteByteReady() {
    return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

static unsigned int uart_isReadByteReady() {
    return mmio_read(AUX_MU_LSR_REG) & 0x01;
}

void uart_writeByteBlockingActual(unsigned char c) {
    while (!uart_isWriteByteReady());
    mmio_write(AUX_MU_IO_REG, (unsigned int)c);
}

char uart_readByteBlockingActual(void) {
    while (!uart_isReadByteReady());
    return (mmio_read(AUX_MU_IO_REG) & 0xFF);
}

void uart_writeText(char *buffer) {
    while (*buffer) {
       if (*buffer == '\n') uart_writeByteBlockingActual('\r');
       uart_writeByteBlockingActual(*buffer++);
    }
}

// Requirement for printf
void putc(void* p, char c) {
    uart_writeByteBlockingActual(c);
}
