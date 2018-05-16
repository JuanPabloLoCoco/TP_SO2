#include <stdint.h>
#include <syscalls.h>
#include <stdlib.h>
#include <ipc_info.h>
extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

/* SystemCall Read */
int syscall_read(unsigned int fd, void * buffer, int bytes) {
    return _int80((uint64_t)0,(uint64_t)fd,(uint64_t)buffer,(uint64_t)bytes,(uint64_t)0,(uint64_t)0);
}

/* SystemCall Write */
int syscall_write(unsigned int fd, const void *buffer, unsigned int bytes) {
    return _int80((uint64_t)1,(uint64_t)fd,(uint64_t)&buffer,(uint64_t)bytes,(uint64_t)0,(uint64_t)0);
}

int get_screen_info(int op)
{
    return (int)_int80(11, op, 0, 0, 0, 0);
}

void setPointer(int x, int y)
{
  _int80(12, x , y, 0, 0, 0);
}

uint64_t allocMemory(uint64_t amount)
{
	return (void *)_int80(14, amount, 0, 0, 0, 0);
}

void freeMemory(void * address)
{
	_int80(15,(uint64_t)address, 0, 0, 0, 0);
}

void ps(uint64_t address, uint64_t amount)
{
	_int80(17, address, amount, 0, 0, 0);
}


int get_current_pids(int * pid_array)
{
  return _int80h(33, (uint64_t) pid_array, 0, 0, 0, 0);
}

int get_process_info(uint64_t pid, struct process_info_c * pi)
{
  return _int80h(35, pid, (uint64_t) pi, 0, 0, 0);
}

int get_semaphore_info(semaphore_info info_array[])
{
  return _int80h(36, (uint64_t) info_array, 0, 0, 0, 0);
}

int get_mutexes_info(mutex_info info_array[])
{
  return _int80h(37, (uint64_t) info_array, 0, 0, 0 , 0);
}
