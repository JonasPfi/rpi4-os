#ifndef MMIO_H
#define MMIO_H

#include "base.h"

#define PAGE_SHIFT	 	12
#define TABLE_SHIFT 		9
#define SECTION_SHIFT		(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   		(1 << PAGE_SHIFT)	
#define SECTION_SIZE		(1 << SECTION_SHIFT)	

#define LOW_MEMORY              (2 * SECTION_SIZE)
#define HIGH_MEMORY             PERIPHERAL_BASE

#define PAGING_MEMORY           (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES            (PAGING_MEMORY/PAGE_SIZE)


// Read/Write memory
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

unsigned long get_free_page();
void free_page(unsigned long p);

#endif // MMIO_H
