#ifndef PMU_H
#define PMU_H

void init_pmu(void);
unsigned long read_pmu_counter(int counter);
void reset_pmu_counters(void);
void disable_pmu_counters(void);
void enable_pmu_counters(void);

#endif
