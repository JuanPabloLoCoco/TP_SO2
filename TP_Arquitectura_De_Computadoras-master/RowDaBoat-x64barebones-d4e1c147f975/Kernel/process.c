#include "include/lib.h"
#include "include/buddyAllocator.h"
#include "include/videoDriver.h"
#include "include/systemCallDispatcher.h"
#include "include/pipe.h"
#include <defs.h>
#include <mutex.h>
#include <semaphore.h>
#include <strings.h>
#include <process.h>
#include <scheduler.h>

#define MAX_PROCESSES 128

static uint64_t nextPID = 0;
static int table_mutex_key;
static process * process_table[MAX_PROCESSES] = {NULL};
static process * foreground = NULL;
static uint64_t n_processes = 0;

static void unblock_foreground_process (process * p);
static uint64_t fill_stack(uint64_t entryPoint, uint64_t stack_base, uint64_t params);


int equalProcesses(process * p1, process * p2)
{
	return p1->pid == p2->pid;
}

static void lock_table()
{
	mutex_lock(table_mutex_key);
}

static void unlock_table()
{
	mutex_unlock(table_mutex_key);
}

void initialize_process_mutex()
{
	table_mutex_key = mutex_open(PROCESSMUTEX);
}

static int insert_process(process * p)
{
	int i;
	lock_table();
	for (i = 0; i < MAX_PROCESSES; i++)
	{
		if (process_table[i]==NULL)
		{
			n_processes++;
			p->pid = i;
			process_table[i] = p;
			unlock_table();
			return i;
		}
	}
	unlock_table();
	return -1;
}

int get_current_pids(int pid_array[])
{
	int i, j, process_left;
	lock_table();

	process_left = n_processes;
	for (i = j = 0;i<MAX_PROCESSES && process_left > 0; i++)
	{
		if (process_table[i] != NULL)
		{
			pid_array[j++] = pid_process(process_table[i]);
			process_left--;
		}
	}
	pid_array[j] = -1;
	unlock_table();
	return j;
}

int get_name_process(char * buffer, process * p)
{
	return strcpyWithNoIndex(buffer, p->descr);
}

process * createProcess(uint64_t entryPoint, uint64_t params, char * name)
{
	process * newProcess = (process *)buddyAllocate(sizeof(process));
	newProcess->entry_point = entryPoint;
	newProcess->stack_page = get_stack_page();
	newProcess->rsp = fill_stack(entryPoint, newProcess->stack_page, params);
	newProcess->pid = nextPID++;
	for(int i=0;i<5;i++)
	{
		newProcess->fd[i]=0;
	}
	newProcess->state = READY;
	strcpyWithNoIndex (newProcess->descr, name);

	if (newProcess->pid != 0)
	{
		newProcess->ppid = pid_process(get_current_process());
	}
	else
	{
		foreground = newProcess;
	}

	insert_process(newProcess);
	return newProcess;
}

process * get_process_by_pid (uint64_t pid)
{
	if (pid < MAX_PROCESSES && process_table[pid] != NULL && !is_dead_process(process_table[pid]))
	{
		return process_table[pid];
	}

	return NULL;
}

void destroy_process(process * p)
{
	if (p != NULL)
	{
		lock_table();
		n_processes--;
		freeProcess(p);
		if (foreground == p)
		{
			set_foreground_process(process_table[p->ppid]);
		}
		process_table[p->pid] = NULL;
		unlock_table();
	}
}

void freeProcess(process * process)
{
	store_stack_page(process->stack_page);
	buddyFree(process);
}

void * stack_page_process(process * p)
{
	return (void *) p->stack_page;
}

int kill_process(process * p)
{
	if (p != NULL && p->pid > 1)
	{
		p->state = DEAD;
	}
	return p != NULL;
}

int is_dead_process (process * p)
{
	if (p != NULL)
	{
		return p->state == DEAD;
	}
	return 0;
}

void set_rsp_process(process * p, uint64_t rsp)
{
	if (p != NULL)
	{
		p->rsp = rsp;
	}
}

uint64_t get_rsp_process(process * p)
{
	if (p != NULL)
	{
		return p->rsp;
	}
	return -1;
}

uint64_t pid_process(process * p)
{
	if (p != NULL)
	{
		return p->pid;
	}
	return -1;
}

uint64_t ppid_process(process * p)
{
	if (p != NULL)
	{
		return p-> ppid;
	}
	return -1;
}

void block_process(process * p)
{
	if (p != NULL && p->state != DEAD)
	{
		p->state = BLOCKED;
	}
}

void unblock_process(process * p)
{
	if (p != NULL && p->state != DEAD)
	{
		p->state = READY;
	}
}

int is_blocked_process(process * p) {
	if (p != NULL)
		{
			return p->state == BLOCKED || p->state == BLOCKED_READ || p->state == BLOCKED_FOREGROUND;
		}
		return 1;
}

void unblock_read_process(process * p)
{
	if (p->state == BLOCKED_READ)
		{
			unblock_process(p);
		}
}

void block_read_process(process * p)
{
	p->state = BLOCKED_READ;
}

void set_foreground_process (process * p)
{
	if (foreground == get_current_process())
	{
		set_foreground_force_process(p);
	}

}

void set_foreground_force_process(process * p)
{
	if (p != NULL && p->pid != 0)
	{
		foreground = p;
		unblock_foreground_process(p);
	}
}

static void unblock_foreground_process (process * p)
{
	if (p != NULL && p->state == BLOCKED_FOREGROUND)
	{
		p->state = READY;
	}
}

void block_foreground_process(process * p) {
	if (p != NULL)
	{
		p->state = BLOCKED_FOREGROUND;
	}
}

process * get_foreground_process()
{
	return foreground;
}

uint64_t number_processes()
{
	return n_processes;
}

static uint64_t fill_stack(uint64_t entryPoint, uint64_t stack_page, uint64_t params)
{
	StackFrame * frame = (StackFrame *) stack_page - 1;
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	params;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	entryPoint;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return (uint64_t) &frame->gs;
}
