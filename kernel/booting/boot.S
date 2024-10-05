.section ".text.boot"

#define _STACK      0x80000
#define _EXCSTACK   0x40000
.extern _regs
.extern main
.extern dispatch

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, chef_core
    
    // cpu id is not zero:
_halt:
    wfi
    b       _halt

chef_core:  // cpu id == 0

    // config EL1 before leaving inital(!) EL2, exc. handler run in EL1h
    ldr     x1, =_EXCSTACK
    msr     sp_el1, x1
    
    // enable AArch64 in EL1
    mov     x2, #(1 << 31)          // AArch64
    msr     hcr_el2, x2
    
    // Setup System Control register access
    mov     x2, #0x0800             // = more details!
    movk    x2, #0x30d0, lsl #16    // last bit is 0 = MMU disabled (Cache?)
    msr     sctlr_el1, x2
    
    // change execution level to EL1
    mov     x2, #0x3c4
    msr     spsr_el2, x2            // EL1_SP0 | D | A | I | F
    adr     x2, finished_el1
    msr     elr_el2, x2
    eret

finished_el1:
    ldr     x1, =_STACK
    mov     sp, x1           // main thread runs in EL1t and uses sp_el0

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
bss_loop:
    cbz     w2, start_kernel
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, bss_loop

    // set up exception handlers for EL1
    ldr     x2, =_vectors
    msr     vbar_el1, x2
    
start_kernel:
    bl      main
    b       _halt
