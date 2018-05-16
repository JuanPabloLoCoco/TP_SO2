#ifndef IPC_INFO
#define IPC_INFO

#include <mutex.h>
#include <semaphore.h>

typedef struct mutex_info {
	char name[MUTEX_NAME_LEN];
	int key;
	int locked;
} mutex_info;

typedef struct semaphore_info {
	char name[SEMAPHORE_NAME_LEN];
	int key;
} semaphore_info;


int get_semaphore_info(semaphore_info info_array[]);
int get_mutexes_info(mutex_info info_array[]);

#endif
