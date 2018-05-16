#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdint.h>

#define MAX_PROCESS_NAME 64
#define MAX_PROCESS_STRING 256

#define MAX_PROCESSES 128


typedef struct process_info_c {
	char is_foreground;
	char state;
	uint64_t pid;
	uint64_t ppid;
	void * stack_addres;
	char name [MAX_PROCESS_NAME];
}process_info;

/* Hace un toString del proceso. Se pasa un puntero para que sea más eficiente.
** Devuelve la longitud. */
int process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]);

#endif
