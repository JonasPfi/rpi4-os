#include "sysregs.h"

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b
2:  // We're on the main core!

    // Initialize SP_EL1
    msr     sp_el1, x1            // Set stack pointer for EL1

    // Enable CNTP for EL1
    mrs     x0, cnthctl_el2       // Read CNTP control from EL2
    orr     x0, x0, #3            // Set the counter enable bits
    msr     cnthctl_el2, x0       // Write back to CNTP control
    msr     cnthp_ctl_el2, xzr     // Disable CNTHP control

    // Initialize virtual MPIDR
    mrs     x0, midr_el1          // Read MIDR from EL1
    mrs     x2, mpidr_el1         // Read MPIDR from EL1
    msr     vpidr_el2, x0         // Set virtual MPIDR in EL2
    msr     vmpidr_el2, x2        // Set virtual MPIDR in EL2

    // Disable coprocessor traps
    mov     x0, #0x33FF           // Prepare value to disable traps
    msr     cptr_el2, x0          // Write to CPTR to disable traps
    msr     hstr_el2, xzr         // Clear HSTR

    // Enable FP/SIMD at EL1
    mov     x0, #(3 << 20)        // Enable FP/SIMD access at EL1
    msr     cpacr_el1, x0         // Write to CPACR

    // Enable AArch64 mode in EL1
    mov     x0, #(1 << 31)        // Set AArch64 mode
    orr     x0, x0, #(1 << 1)     // Set SWIO
    msr     hcr_el2, x0           // Write to HCR to configure

    // Set SCTLR for EL1
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED // Load SCTLR value
    msr     sctlr_el1, x0         // Write to SCTLR for EL1

    // Set SPSR for the switch to EL1
    ldr     x0, =SPSR_VALUE       // Load SPSR value
    msr     spsr_el2, x0          // Write to SPSR

    // Set target address for EL1 entry
    adr     x0, el1_entry         // Load address of EL1 entry point
    msr     elr_el2, x0           // Set ELR for EL2

    // Change exception level to EL1
    eret                          // Execute return from exception

el1_entry:
    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main
    // In case it does return, halt the master core too
    b       1b
