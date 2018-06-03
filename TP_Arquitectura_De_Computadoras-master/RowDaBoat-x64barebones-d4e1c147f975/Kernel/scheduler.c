#include "include/process.h"
#include "include/lib.h"
#include "include/defs.h"
#include "include/videoDriver.h"
#include "include/mutex.h"
#include "include/scheduler.h"
#include "include/strings.h"
#include "include/KeyboardDriver.h"
#include "include/process.h"
#include "include/interrupts.h"
#include "include/buddyAllocator.h"

#define QUANTUM 1

static processSlot * current = NULL;
static processSlot * prev = NULL;
static int superlock = UNLOCKED;

extern int _unlocked(int64_t * locknum);

static void set_next_current();
static void add_process(process * p);

void set_superlock()
{
	superlock = LOCKED;
}

void unset_superlock()
{
	superlock = UNLOCKED;
}

process * get_current_process()
{
	return current->process;
}

uint64_t next_process (uint64_t current_rsp)
{
	if (current == NULL)
	{
		return current_rsp;
	}

	unassign_quantum();

	if (current->quantum > 0)
	{
		return current_rsp;
	}

	if (!_unlocked(&superlock))
	{
		return current_rsp;
	}

	current->quantum = QUANTUM;
	set_rsp_process(current->process, current_rsp);

	prev = current;
	current = current->next;

	set_next_current();
	unset_superlock();

	return get_rsp_process(current->process);
}

uint64_t exec(process * newProcess)
{
	int pid;
	add_process(newProcess);
	pid = pid_process(newProcess);
	if (pid == 0)
	{
		_change_process(get_rsp_process(current->process));
	}
	return pid;
}

static void add_process(process * p)
{
	processSlot * new_slot = (processSlot *)buddyAllocate(sizeof(* new_slot));
	new_slot->process = p;
	new_slot->quantum = QUANTUM;

	set_superlock();
	if (current == NULL)
	{
		current = new_slot;
		current->next = current;
		prev = current;
	}
	else
	{
		new_slot->next = current->next;
		current->next = new_slot;
	}
	unset_superlock();
}

void yield_process()
{
	/* Quantum al siguiente proceso pues el actual quitó tiempo */
	current->next->quantum += 1;
	current->quantum = 0;
	_yield_process();
}

void end_process()
{
	set_superlock();
	processSlot * n = current;
	destroy_process(n->process);
	prev->next = current->next;
	current = current->next;
	set_next_current();
	unset_superlock();
	buddyFree(n);
	/* Se le da un quantum al nuevo proceso */
	assign_quantum();
	_change_process(get_rsp_process(current->process));
}

static void set_next_current()
{
	while (is_blocked_process(current->process) || is_dead_process(current->process))
	{
		processSlot * next = current->next;
		if (is_dead_process(current->process))
		{
			prev->next = next;
			destroy_process(current->process);
			buddyFree (current);
		}
		else
		{
			prev = current;
		}
		current = next;
	}
}

void assign_quantum()
{
	current->quantum += 1;
}

void unassign_quantum()
{
	current->quantum -= 1;
}
