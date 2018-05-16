#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdint.h>

#define MAX_PROCESS_NAME 64
#define MAX_PROCESS_STRING 256

#define MAX_PROCESSES 128

#define SEPARATOR ' '
#define FOREGROUND "foreground"
#define BACKGROUND "background"
#define STACK_ADDR "Stack Address: "
#define MEM_ADDR "Memory Address: "
#define PPID "PPID"

typedef enum processState_t {RUNNING, READY, BLOCKED, BLOCKED_READ, DEAD, BLOCKED_FOREGROUND} processState;

typedef struct process_info_c {
	char is_foreground;
	char state;
	uint64_t pid;
	uint64_t ppid;
	void * stack_address;
	char name [MAX_PROCESS_NAME];
}process_info;

/* Hace un toString del proceso. Se pasa un puntero para que sea más eficiente.
** Devuelve la longitud. */
int process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]);

#endif
