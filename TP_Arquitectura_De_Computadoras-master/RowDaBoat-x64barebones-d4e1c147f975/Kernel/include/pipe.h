#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include "semaphore.h"

#include "IPCstruct.h"

#define MAX_PIPES 100
#define MAX_PIPE_NAME 15
#define PIPES_MUTEX "__PIPES__MUTEX"

typedef struct{
    char buffer[1024*4];
    int bufferSize;
    int initialIndex;
    int mutex;
    int readMutex;
    int writeMutex;
    int readSemaphore;
    int writeSemaphore;
    char* name;
}pipe_s;

typedef pipe_s * pipe_t;

pipe_t getPipe(char* name);
void releasePipe(char* name);
int writePipe(pipe_t pipe,char* msg , uint64_t amount);
int readPipe(pipe_t pipe,char* ans,uint64_t amount);
void initIPC();
int whereIsPipe(char* name);
int nextfreePipe(char* name);
void deletePipe(pipe_t pipe);
int getPipesNames(ipcs* ans,int cant);
int addPipe(pipe_t p);


#endif
