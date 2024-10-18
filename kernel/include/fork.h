#ifndef FORK_H
#define FORK_H

#ifndef __ASSEMBLER__

int copy_process(unsigned long fn, unsigned long arg);
void ret_from_fork(void);

#endif

#endif
