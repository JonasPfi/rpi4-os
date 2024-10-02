#ifndef MMIO_H
#define MMIO_H

// MMIO Funktionen
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

#endif // MMIO_H
