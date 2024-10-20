#include "base.h"

void init_pmu(void) {
    // Activate PMU and reset
    unsigned long val;

    asm volatile("mrs %0, PMCR_EL0" : "=r"(val));     // D10.4.7
    val |= (1 << 0);  // All counters that are accessible at Non-secure EL1 are enabled by PMCNTENSET_EL0
    val |= (1 << 2);  // Reset PMCCNTR_EL0 to zero
    asm volatile("msr PMCR_EL0, %0" : : "r"(val));    // Write PMCR_EL0

    asm volatile("msr PMSELR_EL0, %0" : : "r"(0));    // D10.4.14: Selects first counter
    asm volatile("msr PMXEVTYPER_EL0, %0" : : "r"(0x08));  // D10.4.18 -> Instruction architecturally executed

    // Activate counter
    asm volatile("msr PMCNTENSET_EL0, %0" : : "r"(1 << 0));  // D10.4.6
}

unsigned long read_instruction_count(void) {
    unsigned long count;
    asm volatile("msr PMSELR_EL0, %0" : : "r"(0));    // D10.4.14: Selects first counter
    asm volatile("mrs %0, PMXEVCNTR_EL0" : "=r"(count));  // D10.4.17
    return count;
}

void reset_instruction_count(void) {
    asm volatile("msr PMSELR_EL0, %0" : : "r"(0));
    asm volatile("msr PMXEVCNTR_EL0, %0" : : "r"(0));
}

void disable_pmu_counter(void) {
    asm volatile("msr PMCNTENCLR_EL0, %0" : : "r"(1 << 0));  // Deactivate instruction counter
}

void enable_pmu_counter(void) {
    asm volatile("msr PMCNTENSET_EL0, %0" : : "r"(1 << 0));  // Activate instruction counter
}
