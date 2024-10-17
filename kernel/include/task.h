#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct {
    uint64_t sp;      // Stackpointer
    uint64_t regs[31]; // Register
    uint64_t pc;      // Program Counter
} task_t;

#endif // TASK_H
