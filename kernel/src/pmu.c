#include "base.h"

void init_pmu(void) {
    // Activate PMU and reset
    unsigned long val;

    asm volatile("mrs %0, PMCR_EL0" : "=r"(val));     // D10.4.7
    val |= (1 << 0);  // Enable all counters accessible at Non-secure EL1 (PMCNTENSET_EL0)
    val |= (1 << 2);  // Reset PMCCNTR_EL0 to zero
    asm volatile("msr PMCR_EL0, %0" : : "r"(val));    // Write PMCR_EL0

    // Configure counters
    unsigned int events[] = {0x03,  // L1D_CACHE_REFILL (D10.4.18)
                              0x08,  // INST_RETIRED (D10.4.18)
                              0x10,  // BRANCH_MISPRED (D10.4.18)
                              0x13,  // MEM_ACCESS (D10.4.18)
                              0x11}; // CPU_CYCLES (D10.4.18)

    for (int i = 0; i < 5; i++) {
        asm volatile("msr PMSELR_EL0, %0" : : "r"(i));  // Select counter i (D10.4.14)
        asm volatile("msr PMXEVTYPER_EL0, %0" : : "r"(events[i]));  // Set event type (D10.4.18)
    }

    // Activate all counters
    asm volatile("msr PMCNTENSET_EL0, %0" : : "r"(0x1F));  // Enable counters (D10.4.6)
}

unsigned long  read_pmu_counter(int counter) {
    unsigned long count;
    asm volatile("msr PMSELR_EL0, %0" : : "r"(counter));  // Select counter (D10.4.14)
    asm volatile("mrs %0, PMXEVCNTR_EL0" : "=r"(count));  // Read counter (D10.4.17)
    return count;
}

void reset_pmu_counters(void) {
    for (int i = 0; i < 5; i++) {
        asm volatile("msr PMSELR_EL0, %0" : : "r"(i));
        asm volatile("msr PMXEVCNTR_EL0, %0" : : "r"(0));
    }
}

void disable_pmu_counters(void) {
    asm volatile("msr PMCNTENCLR_EL0, %0" : : "r"(0x1F));  // Deactivate all counters
}

void enable_pmu_counters(void) {
    asm volatile("msr PMCNTENSET_EL0, %0" : : "r"(0x1F));  // Activate all counters
}
