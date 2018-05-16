#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "process.h"
#include "defs.h"

typedef struct processSlot_t{
	process * process;
	int quantum;
	struct processSlot_t * next;
}processSlot;

uint64_t next_process(uint64_t current_rsp);

uint64_t exec(process * new_process);
void end_process();
void yield_process();

process * get_current_process();

void assign_quantum();
void unassign_quantum();

void set_superlock();
void unset_superlock();

#endif
