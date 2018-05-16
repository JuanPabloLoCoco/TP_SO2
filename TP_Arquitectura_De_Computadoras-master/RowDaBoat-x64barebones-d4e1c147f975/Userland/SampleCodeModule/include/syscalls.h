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

int get_screen_info(int op);

void setPointer(int x, int y);

uint64_t allocMemory(uint64_t amount);

void freeMemory(void * address);

void ps(uint64_t address, uint64_t amount);


#endif
