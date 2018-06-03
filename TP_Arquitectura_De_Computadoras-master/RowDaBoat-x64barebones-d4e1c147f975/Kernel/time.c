#include <time.h>
#include <naiveConsole.h>
#include <mutex.h>
#include <process.h>
#include <scheduler.h>
#include <defs.h>
#include <buddyAllocator.h>

static sleep_process * sleeping_process = NULL;
static sleep_process * create_sleep_process(uint64_t ticks);
static void add_sleep_process(sleep_process * p);


/* Controla la cantidad de ticks del timer tick */

void timer_handler()
{
	sleep_process * current = sleeping_process;
	sleep_process * prev = NULL;
	sleep_process * sp;
	while (current != NULL)
	{
		current->ticks -=1;
		if (current->ticks <= 0)
		{
			sp = current;
			if (prev == NULL)
			{
				sleeping_process = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			unblock_process(current->process);
			current = current-> next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
/* Bloquea el proceso actual y lo pone en una lista con los ticks necesarios hasta despertar */
int sleep(uint64_t milliseconds)
{
	uint64_t wait_ticks = milliseconds / 55;
	// Hay 1 tick cada 55 milisegundos aproximadamente
	if (wait_ticks == 0)
	{
		return 0;

	}

	assign_quantum();
	sleep_process * new_sleep_p = create_sleep_process(wait_ticks);
	add_sleep_process(new_sleep_p);
	block_process(new_sleep_p->process);
	yield_process();
	return 1;
}

static sleep_process * create_sleep_process(uint64_t ticks)
{
	sleep_process * new_sleep_p = (sleep_process*) buddyAllocate(sizeof(*new_sleep_p));
	new_sleep_p->ticks = ticks;
	new_sleep_p->process = get_current_process();
	return new_sleep_p;
}

static void add_sleep_process(sleep_process * p)
{
	p->next = sleeping_process;
	sleeping_process = p;
}
