#ifndef _IPC_INFO_
#define _IPC_INFO_

#define MAX_SEMAPHORE 128
#define MAX_MUTEXES 128

#define MUTEX_NAME_LEN 64
#define SEMAPHORE_NAME_LEN 64

typedef struct mutex_info {
	char name[MUTEX_NAME_LEN];
	int key;
	int locked;
} mutex_info;

typedef struct semaphore_info {
	char name[SEMAPHORE_NAME_LEN];
	int key;
} semaphore_info;

void print_semaphore_info(semaphore_info info_array[], int count);
void print_mutexes_info(mutex_info info_array[], int count);
#endif
