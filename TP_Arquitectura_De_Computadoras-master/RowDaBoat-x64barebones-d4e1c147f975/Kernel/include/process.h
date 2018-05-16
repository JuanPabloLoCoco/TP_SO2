#ifndef PROCESS_H
#define PROCESS_H

#include "pipe.h"

#define MAX_PROCESS_NAME 64
#define INIT_PROCESS_PAGES 4

typedef enum processState_t {RUNNING, READY, BLOCKED, BLOCKED_READ, DEAD, BLOCKED_FOREGROUND} processState;

typedef struct {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} StackFrame;

typedef struct process_t{
	uint64_t entry_point;		// instruction pointer
	uint64_t stack_page;		// beggining of stack's pages
	uint64_t rsp;	// relative to stack_base
	processState state;
	uint64_t pid;
	uint64_t ppid;
	char descr[MAX_PROCESS_NAME];
	pipe_t fd[5];
} process;

/* Returns 1 if p1 is the same process as p2* and 0 if different */
int equalProcesses(process * p1, process * p2);

void freeProcess(process * process);
process * createProcess(uint64_t entryPoint,uint64_t params, char* name);
void initialize_process_mutex();

void destroy_process(process * p);

void set_rsp_process(process * p, uint64_t rsp);
uint64_t get_rsp_process(process * p);
process * get_process_by_pid (uint64_t pid);
/* Devuelve los pids actuales. Termina el arreglo con -1 */
int get_current_pids(int pid_array[]);
uint64_t pid_process(process * p);
uint64_t ppid_process(process * p);
uint64_t number_processes();
int get_name_process(char * buffer, process * p);
void * stack_page_process(process * p);

void block_process(process * p);
void unblock_process(process * p);
int is_blocked_process(process * p);
void unblock_read_process(process * p);
void block_read_process(process * p);
void block_foreground_process(process * p); /* Se desbloquea al ahcer set_foreground */

/* Quizas no tengan que estar aca */
void set_foreground_process (process *p);
void set_foreground_force_process (process * p);
process * get_foreground_process();

/* kill settea que hay que borrar el proceso. No lo borra. is_delete devuelve 1 si hay que borrarlo. */
int kill_process(process * p);
int is_dead_process(process * p);

#endif
