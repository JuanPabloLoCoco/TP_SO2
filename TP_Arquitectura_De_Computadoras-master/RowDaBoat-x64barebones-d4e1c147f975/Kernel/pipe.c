#include "include/pipe.h"
#include "include/buddyAllocator.h"
#include "include/lib.h"
#include "include/strings.h"
#include "include/mutex.h"
#include "include/scheduler.h"
#include "include/videoDriver.h"
#include <semaphore.h>

static pipe_t pipes[MAX_PIPES];
static int pipeMutex;

int getPipesNames(ipcs* ans,int cant)
{
    int j;
    int i;
    for (i = 0,j=0; j < cant && i<MAX_PIPES; ++i)
    {
        if(pipes[i]!=(pipe_t )0)
        {
            strcpy(ans->pipesNames[j],pipes[i]->name,MAX_PIPE_NAME);
            j++;
        }
    }
    return j;
}

void initIPC()
{
    for (int i = 0; i < MAX_PIPES; ++i)
    {
        pipes[i]=(pipe_t)0;

    }
    pipeMutex = mutex_open(PIPES_MUTEX);
}

pipe_t createPipe(char* name)
{
    if(name[0]=='\0') return (pipe_t)0;
    char mname[16]={'p','_'};
    strcpy(mname+1,name,13);
    mname[15]='\0';
    pipe_t newPipe= buddyAllocate(sizeof(* newPipe));
    newPipe->name=buddyAllocate(MAX_PIPE_NAME+1);
    strcpy(newPipe->name,name,MAX_PIPE_NAME);

    newPipe->mutex = mutex_open(mname);
    newPipe->buffer = buddyAllocatePages(1);
    newPipe->bufferSize = 0;
    newPipe->initialIndex = 0;

    mname[0]='R';
    newPipe->readMutex = mutex_open(mname);
    mname[0]='W';
    newPipe->writeMutex = mutex_open(mname);

    mname[0]='R';
    newPipe->readSemaphore = semaphore_open(mname);
    mname[0]='W';
    newPipe->writeSemaphore = semaphore_open(mname);

    return newPipe;
}

int addPipe(pipe_t p)
{
    if(p==0)
    {
      return 0;
    }

    process * me= get_current_process();
    int i;
    for (i = 0; i < 5; ++i)
    {
        if(me->fd[i]==0)
        {
            me->fd[i]=p;
            break;
        }
        else if(me->fd[i] == p)
        {
            break;
        }
    }
    return (i==5)?-1:i;
}

pipe_t getPipe(char * name)
{
    mutex_lock(pipeMutex);

    int pos = whereIsPipe(name);
    if(pos != -1 )
    {
        mutex_unlock(pipeMutex);
        return pipes[pos];
    }

    pos = nextfreePipe(name);
    if(pos != -1)
    {
        pipes[pos]= createPipe(name);
        mutex_unlock(pipeMutex);
        return pipes[pos];
    }
    mutex_unlock(pipeMutex);
    return (pipe_t ) 0;
}

int whereIsPipe(char* name)
{
    for (int i = 0; i < MAX_PIPES; ++i)
    {
        if(pipes[i]!= 0 && strcmp(pipes[i]->name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void releasePipe(char* name)
{
    mutex_lock(pipeMutex);
    int pos = whereIsPipe(name);
    if(pos != -1)
    {
        deletePipe((pipe_t )pipes[pos]);
        pipes[pos] = 0;
    }
    mutex_unlock(pipeMutex);
}

int nextfreePipe(char* name)
{
    int i;
    for(i=0 ;i < MAX_PIPES ;i++)
    {
        if(pipes[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

void deletePipe(pipe_t pipe)
{
    buddyFree(pipe->name);
    buddyFree(pipe->buffer);
    mutex_close(pipe->writeMutex);
    mutex_close(pipe->readMutex);
    mutex_close(pipe->mutex);
    buddyFree(pipe);
    return;
}

int writePipe(pipe_t pipe,char* msg, uint64_t amount)
{
    int i;
    mutex_lock(pipe->writeMutex);
    while (pipe->bufferSize >= MINPAGE)
    {
        semaphore_wait(pipe->writeSemaphore,pipe->writeMutex);
    }
    mutex_lock(pipe->mutex);
    for(i=0; i< amount;i++)
    {
        pipe->buffer[(pipe->initialIndex + pipe->bufferSize) %MINPAGE] = msg[i];
        pipe->bufferSize ++;
    }
    semaphore_signal(pipe->readSemaphore);
    mutex_unlock(pipe->mutex);
    mutex_unlock(pipe->writeMutex);
    return 1;
}

int readPipe(pipe_t pipe,char* ans,uint64_t amount)
{
    int j;
    mutex_lock(pipe->readMutex);
    while (pipe->bufferSize <= 0)
    {
        semaphore_wait(pipe->readSemaphore,pipe->readMutex);
    }

    mutex_lock(pipe->mutex);
    for(j=0; j < amount && pipe->bufferSize > 0;j++)
    {
        ans[j] = pipe->buffer[pipe->initialIndex%MINPAGE];
        pipe->initialIndex = (pipe->initialIndex + 1)%MINPAGE;
        pipe->bufferSize--;
    }
    semaphore_signal(pipe->writeSemaphore);
    mutex_unlock(pipe->mutex);

    mutex_unlock(pipe->readMutex);
    return 1;
}
