#include "scheduler.h"
#include "uart.h"

.globl cpu_switch_to
cpu_switch_to:
    mov x10, #THREAD_CPU_CONTEXT
    add x8, x0, x10
    mov x9, sp

    // Store callee-saved registers
    str x19, [x8], #8
    str x20, [x8], #8
    str x21, [x8], #8
    str x22, [x8], #8
    str x23, [x8], #8
    str x24, [x8], #8
    str x25, [x8], #8
    str x26, [x8], #8
    str x27, [x8], #8
    str x28, [x8], #8
    str x29, [x8], #8
    str x9, [x8], #8
    str x30, [x8]

    // Restore callee-saved registers
    add x8, x1, x10
    ldr x19, [x8], #8
    ldr x20, [x8], #8
    ldr x21, [x8], #8
    ldr x22, [x8], #8
    ldr x23, [x8], #8
    ldr x24, [x8], #8
    ldr x25, [x8], #8
    ldr x26, [x8], #8
    ldr x27, [x8], #8
    ldr x28, [x8], #8
    ldr x29, [x8], #8
    ldr x9, [x8], #8
    ldr x30, [x8]

    mov sp, x9
    ret
