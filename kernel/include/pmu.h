#ifndef PMU_H
#define PMU_H

void init_pmu(void);
unsigned long read_instruction_count(void);
void reset_instruction_count(void);
void disable_pmu_counter(void);
void enable_pmu_counter(void);

#endif
