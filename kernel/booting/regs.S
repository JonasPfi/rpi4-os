.section ".text.boot"
.global _regs_save
.global _regs_load

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


