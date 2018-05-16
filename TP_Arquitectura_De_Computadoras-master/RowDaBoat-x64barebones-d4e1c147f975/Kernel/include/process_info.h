#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include "process.h"
#include "scheduler.h"
#include "lib.h"
#include <stdint.h>

typedef struct process_info_c {
	char is_foreground;
	char state;
	uint64_t pid;
	uint64_t ppid;
	void * stack_addres;
	char name [MAX_PROCESS_NAME];
	pipe_t fd[5];
}process_info;

/* Copia la información de un proceso a la estructura pública process_info.
** Dicha estructura funciona como interfaz entre Kernel y Userland para leer
** la información de un proceso */
void get_process_info (process_info * pi, process * p);

int get_process_info_by_pid(process_info * pi, uint64_t pid);
#endif
