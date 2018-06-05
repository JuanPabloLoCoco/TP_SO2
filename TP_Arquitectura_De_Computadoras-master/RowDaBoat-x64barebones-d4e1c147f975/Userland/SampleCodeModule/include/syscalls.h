#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#include "process_info.h"
#include "ipc_info.h"

#define STDOUT 0
#define STDIN 1
#define STDERR 2


/* SystemCall Read */
int syscall_read(unsigned int fd, void * buffer, int bytes);
/* SystemCall Write */
int syscall_write(unsigned int fd, const void *buffer, unsigned int bytes);

int get_seconds();

int get_minutes();

int get_hours();

int get_year();

int get_month();

int get_day();

void sys_paint(int x, int y, uint64_t blue ,uint64_t green, uint64_t red);


void clear();

void sys_drawCharPosition(char * c, int x, int y);

int get_screen_info(int op);

void setPointer(int x, int y);

void color(int blue, int green, int red);

void * allocMemory(uint64_t amount);

void freeMemory(void * address);

void end();

void sleep(int milliseconds);

void yield();

int kill(int pid);

int exec(void * ptr, uint64_t params, const char * name);

int ppid();

int pid();

int openPipe(char * name);

void closePipe(char * name);

int semaphore_signal(int key);

int semaphore_wait(int semaphore_key, int lock_key);

int semaphore_open(char * name);

int semaphore_broadcast(int key);

int mutex_lock(int key);

int mutex_unlock(int key);

int mutex_open(char * name);

int mutex_close(int key);

int get_current_pids(int * pid_array);

int set_foreground(int pid);

int get_process_info(int pid, struct process_info_c * pi);

int get_semaphore_info(semaphore_info info_array[]);

int get_mutexes_info(mutex_info info_array[]);

int semaphore_close(int key);

#endif
