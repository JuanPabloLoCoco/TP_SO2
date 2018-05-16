#include <process_info.h>
#include <syscalls.h>
#include <stdlib.h>

#define SEPARATOR ' '
#define FOREGROUND "foreground"
#define BACKGROUND "background"
#define STACK_ADDR "Stack Address: "
#define MEM_ADDR "Memory Address: "
#define PPID "PPID"

static char * state[] = {"RUNNING", "READY", "BLOCKED"};

static int strcpysep (char * to, const char * source, char separator);


int process_string(const process_info * p, char buffer[MAX_PROCESS_STRING])
{
	int i = 0;
	char * ground = p->is_foreground ? FOREGROUND : BACKGROUND;

	i += itoa(p->pid, buffer + i, 10);

	buffer[i++] = SEPARATOR;

	i += strcpysep(buffer + i, p->name, SEPARATOR); /* nombre */

	i += strcpysep(buffer + i, PPID, SEPARATOR);

	i += itoa(p->ppid, buffer + i, 10);

	buffer[i++] = SEPARATOR;

	i += strcpysep(buffer + i, ground, SEPARATOR); /* [background|foregound] */

	i += strcpysep(buffer + i, state[(int)p->state], SEPARATOR); /* [RUNNING|READY|BLOCKED]*/

	i += strcpysep(buffer + i, STACK_ADDR, SEPARATOR);

	i += itoa((uint64_t) p->stack_address, buffer + i, 16);

	buffer[i++] = SEPARATOR;

	buffer[i] = '\0';
	return i;
}

static int strcpysep (char * to, const char * source, char separator)
{
	int len = strcpy(to, source);
	to[len++] = separator;
	to[len] = '\0';
	return len;
}
