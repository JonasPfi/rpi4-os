.section ".text.boot"
.global _vectors

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
