#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#define MAX_SEMAPHORE 128

#define SEMAPHORE_NAME_LEN 64

#define NOT_OPEN_ERROR -1
#define MAX_SEMAPHORE_OPEN_ERROR -2
#define SEMAPHORE_IDENTIFIER "__SEMAPHORE_LOCK__"

#define OPEN 1
#define CLOSED 0

int initialize_semaphore();

int semaphore_open(char * name);
int semaphore_wait(int semaphore_key, int lock_key);
int semaphore_signal(int key);
int semaphore_broadcast(int key);
int semaphore_close(int key);


#endif
