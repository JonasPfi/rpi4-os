#ifndef TIMER_H
#define TIMER_H

void timer_init(void);
void timer_handler(void);
void timer_start(void);

// Pi4 B, BCM2711, 4MB Base Addresses
#define MMIO_BASE       0xFE000000

// System Timer (and Comparator 3)
#define PIT_STATUS      (MMIO_BASE+0x00003000)
#define PIT_LOW         (MMIO_BASE+0x00003004)
#define PIT_Compare3    (MMIO_BASE+0x00003018)
#define PIT_SPI         99
#define PIT_MASKBIT     3

// IRQ and GIC-400
#define GIC_IRQS        192
#define GIC_SPURIOUS    1023
#define GICD_BASE       0xFF841000
#define GICD_CTLR       (GICD_BASE + 0x000)
#define GICD_ENABLE     (GICD_BASE + 0x100)
#define GICD_DISABLE    (GICD_BASE + 0x180)
#define GICD_PEND_CLR   (GICD_BASE + 0x280)
#define GICD_ACTIVE_CLR (GICD_BASE + 0x380)
#define GICD_PRIO       (GICD_BASE + 0x400)
#define GICD_TARGET     (GICD_BASE + 0x800)
#define GICD_CFG        (GICD_BASE + 0xc00)
// GIC-400 CPU Interface Controller
#define GICC_BASE       0xFF842000
#define GICC_CTLR       (GICC_BASE + 0x000)
#define GICC_PRIO       (GICC_BASE + 0x004)
#define GICC_ACK        (GICC_BASE + 0x00c)
#define GICC_EOI        (GICC_BASE + 0x010)

#endif // TIMER_H
