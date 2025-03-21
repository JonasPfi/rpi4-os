#include "scheduler.h"
#include "irq.h"
#include "printf.h"
#include "pmu.h"
#include "timer.h"
#define TIMER_FREQUENCY 10// Hz
#define STATIC_ENERGY_PER_TICK (((unsigned long)2195 * 1000000UL) / TIMER_FREQUENCY) // nW
static struct task_struct init_task = INIT_TASK;
struct task_struct *current = &(init_task);
struct task_struct * task[NR_TASKS] = {&(init_task), };
int nr_tasks = 1;
unsigned long total_energy_consumed = 0; 
int executed_tasks = 0; // Counts tasks executed in current cycle
int current_cycle_quota = 0; // Passed quotas in current cycle

void preempt_disable(void)
{
	current->preempt_count++;
}

void preempt_enable(void)
{
	current->preempt_count--;
}

void update_energy_usage(struct task_struct *task) {
    unsigned long inst_retired = read_pmu_counter(1);
    unsigned long mem_access = read_pmu_counter(3);

    unsigned long energy_used_inst = (inst_retired * 45 * 1000000) / 10000000UL;
    unsigned long energy_used_mem = (mem_access * 275 * 1000000) / 10000000UL;
    unsigned long energy_used = energy_used_inst + energy_used_mem + STATIC_ENERGY_PER_TICK;

    task->energy_consumed += energy_used;
    total_energy_consumed += energy_used;
    executed_tasks++;

    printf("Task %d: Used %u nW in this quota (Total: %u nW)\n\r",
           task->id, energy_used, task->energy_consumed);

    reset_pmu_counters();
}

void update_task_priorities() {
    unsigned long avg_energy = get_average_energy();

    printf("Average energy: %u nW\n\r", avg_energy);

    for (int i = 1; i < NR_TASKS; i++) {
        struct task_struct *p = task[i];
        if (!p) continue;

	long deviation = (((long)p->energy_consumed / p->quota) - (long)avg_energy) * 100 / (long)avg_energy;
        
	if (deviation >= -5 && deviation <= 5) {
            p->priority = 3;
            p->quota = 2;
        } else if (deviation > 5 && deviation <= 10) {
            p->priority = 4;
            p->quota = 1;
        } else if (deviation < -5 && deviation >= -10) {
            p->priority = 2;
            p->quota = 3;
        } else if (deviation > 10 && deviation <= 20) {
            p->priority = 5;
            p->quota = 1;
        } else {
            p->priority = 1;
            p->quota = 4;
        }

        p->remaining_quota = p->quota;

        printf("Task %d: Energy %u nW, Deviation %d%%, Priority %d, Quota %d\n\r", 
               i, p->energy_consumed, deviation, p->priority, p->quota);
	
	p->energy_consumed = 0;
    }

    total_energy_consumed = 0;
    executed_tasks = 0;
}

unsigned long get_average_energy() {
    if (executed_tasks == 0) return STATIC_ENERGY_PER_TICK;
    return total_energy_consumed / executed_tasks;
}

void _schedule(void) {
    preempt_disable();
    
    if (current_cycle_quota == 0) {
        update_task_priorities();
        current_cycle_quota = 0;
        for (int i = 0; i < NR_TASKS; i++) {
            if (task[i]) {
                current_cycle_quota += task[i]->quota;
            }
        }
    }

    struct task_struct *next_task;
    while (1) {
        int highest_priority = -1;
        
        for (int i = 0; i < NR_TASKS; i++) {
            struct task_struct *p = task[i];
            if (p && p->state == TASK_RUNNING && p->remaining_quota > 0) {
                if (p->priority > highest_priority) {
                    highest_priority = p->priority;
                    next_task = p;
                }
            }
        }

        if (next_task) {
            next_task->remaining_quota--;
            current_cycle_quota--;
            break;
        }

        update_task_priorities(); 
    }

    switch_to(next_task);
    preempt_enable();
}

void schedule(void)
{
	current->counter = 0;
	_schedule();
}

void switch_to(struct task_struct * next) 
{
	if (current == next) 
		return;
	struct task_struct * prev = current;
	current = next;
	cpu_switch_to(prev, next);
}

void schedule_tail(void) {
	preempt_enable();
}

void timer_tick() {
    update_energy_usage(current);
    
    --current->counter;
    if (current->counter > 0 || current->preempt_count > 0) {
        return;
    }

    current->counter = 0;
    _schedule();
}
