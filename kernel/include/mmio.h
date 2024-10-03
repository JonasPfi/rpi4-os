#ifndef MMIO_H
#define MMIO_H

// Read/Write memory
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

// Get Exception level
unsigned int get_el(void);

#endif // MMIO_H
