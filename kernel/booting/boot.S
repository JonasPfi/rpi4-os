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

.global PUT32
PUT32:
  str w1,[x0]
  ret

.global GET32
GET32:
    ldr w0,[x0]
ret

/// @todo what is about the floating point registers!?
_regs_save:
    str     x0, [sp, #-16]!     /* push org x0 value */
    ldr     x0, =_regs+8
    str     x1, [x0], #8        /* _regs[1]=x1 */
    ldr     x1, [sp], #16       /* pop org x0 value => save in x1 */
    str     x1, [x0, #-16]      /* x0-16byte = _regs[0]=x1 (value of x0) */
    str     x2, [x0], #8        /* _regs[2]=x2 */
    str     x3, [x0], #8        /* ...etc. */
    str     x4, [x0], #8
    str     x5, [x0], #8
    str     x6, [x0], #8
    str     x7, [x0], #8
    str     x8, [x0], #8
    str     x9, [x0], #8
    str     x10, [x0], #8
    str     x11, [x0], #8
    str     x12, [x0], #8
    str     x13, [x0], #8
    str     x14, [x0], #8
    str     x15, [x0], #8
    str     x16, [x0], #8
    str     x17, [x0], #8
    str     x18, [x0], #8
    str     x19, [x0], #8
    str     x20, [x0], #8
    str     x21, [x0], #8
    str     x22, [x0], #8
    str     x23, [x0], #8
    str     x24, [x0], #8
    str     x25, [x0], #8
    str     x26, [x0], #8
    str     x27, [x0], #8
    str     x28, [x0], #8
    ldp     x1, x2, [sp], #16       // pop x29, x30
    stp     x1, x2, [x0], #16
    mrs     x1, sp_el0
    str     x1, [x0], #8        // _regs[31] = sp_el0
    mrs     x1, elr_el1
    str     x1, [x0], #8        // _regs[32] = elr_el1
    mrs     x1, spsr_el1        // the other registers for debugging/bluescreen
    str     x1, [x0], #8        // 33
    mrs     x1, esr_el1
    str     x1, [x0], #8        // 34
    mrs     x1, far_el1
    str     x1, [x0], #8        // 35
    mrs     x1, sctlr_el1
    str     x1, [x0], #8        // 36
    mrs     x1, tcr_el1
    str     x1, [x0], #8        // 37
    ret                         // ret uses x0 as return value and uses x30 as return address

/// @todo what is about the floating point registers!?
_regs_load:
    ldr     x0, =_regs+8
    ldr     x1, [x0], #8
    ldr     x2, [x0], #8
    ldr     x3, [x0], #8
    ldr     x4, [x0], #8
    ldr     x5, [x0], #8
    ldr     x6, [x0], #8
    ldr     x7, [x0], #8
    ldr     x8, [x0], #8
    ldr     x9, [x0], #8
    ldr     x10, [x0], #8
    ldr     x11, [x0], #8
    ldr     x12, [x0], #8
    ldr     x13, [x0], #8
    ldr     x14, [x0], #8
    ldr     x15, [x0], #8
    ldr     x16, [x0], #8
    ldr     x17, [x0], #8
    ldr     x18, [x0], #8
    ldr     x19, [x0], #8
    ldr     x20, [x0], #8
    ldr     x21, [x0], #8
    ldr     x22, [x0], #8
    ldr     x23, [x0], #8
    ldr     x24, [x0], #8
    ldr     x25, [x0], #8
    ldr     x26, [x0], #8
    ldr     x27, [x0], #8
    ldr     x28, [x0], #8
    ret                         // uses x0 as return value, uses x30 as return address

    .balign 0x800   // vector must be propper aligned!
_vectors:
    // EL1t ----- SP from EL0 ----------------------------------------
    
    .balign 0x80    // Debug (synchron to memory or instr. "stream")
    eret            // ignore it

    // 0x80 aligned = max 32 instructions a 32 bit
    .balign 0x80    // IRQ (Asynchron)
    stp     x29, x30, [sp, #-16]!    // push x29, x30
    bl      _regs_save        // after that SP is at the same place than before
    bl      dispatch
    // load x1..x28
    bl      _regs_load
    // load x30 (=original return addr of interrupted task) from addr in x0, and add #8 to x0
    ldp     x29, x30, [x0], #16
    ldr     x1, [x0], #8        // _regs[31] with SP maybe changed by "dispatch()"
    msr     sp_el0, x1
    // load lr (=return addr into interrupted task) from addr in x0
    ldr     x0, [x0]            // _regs[32] with LR maybe changed by "dispatch()"
    msr     elr_el1, x0
    // finally load x0,x1 into x0,x1(again)
    ldr     x0, =_regs
    ldp     x0, x1, [x0]
    eret
    
    .balign 0x80    // FIQ (Asynchron)
    eret            // ignore it

    .balign 0x80    // SError (Asynchron)
    eret            // ignore it
