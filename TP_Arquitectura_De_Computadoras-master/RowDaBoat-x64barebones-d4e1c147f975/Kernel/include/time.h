#ifndef TIME_H
#define TIME_H

#include <process.h>

typedef struct c_sleep_process {
	process * process;
	uint64_t ticks;
	struct c_sleep_process * next;
} sleep_process;

void timer_handler();
int sleep(uint64_t milliseconds);

#endif
