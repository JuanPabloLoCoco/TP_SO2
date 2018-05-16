#ifndef MUTEX_H
#define MUTEX_H

#include "IPCstruct.h"
#include "defs.h"

#define MALLOCMUTEX "malloc__mutex"
#define PAGESMUTEX "pages__mutex"
#define PROCESSMUTEX "PROCESS_TABLE_MUTEX"

#define MAX_MUTEXES 128
#define MUTEX_NAME_LEN 64

#define OPEN 1
#define CLOSED 0

#define NOT_OPEN_ERROR -1
#define MAX_MUTEX_OPEN_ERROR -2

int mutex_open(char * name);
int mutex_lock(int key);
int mutex_unlock(int key);
int mutex_close(int key);

#endif
