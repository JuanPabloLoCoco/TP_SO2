#include <stdint.h>
#include <syscalls.h>
#include <stdlib.h>
#include <ipc_info.h>
#include <file_info.h>

extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

/* SystemCall Read */
int syscall_read(unsigned int fd, void * buffer, int bytes)
{
    return _int80((uint64_t)0,(uint64_t)fd,(uint64_t)buffer,(uint64_t)bytes,(uint64_t)0,(uint64_t)0);
}

/* SystemCall Write */
int syscall_write(unsigned int fd, const void *buffer, unsigned int bytes)
{
    return _int80((uint64_t)1,(uint64_t)fd,(uint64_t)buffer,(uint64_t)bytes,(uint64_t)0,(uint64_t)0);
}

int get_seconds()
{
  return (int)_int80(2,0,0,0,0,0);
}

int get_minutes()
{
  return (int)_int80(3,0,0,0,0,0);
}

int get_hours()
{
  return (int)_int80(4,0,0,0,0,0);
}

int get_year()
{
  return (int)_int80(5,0,0,0,0,0);
}

int get_month()
{
  return (int)_int80(6,0,0,0,0,0);
}

int get_day()
{
  return (int)_int80(7,0,0,0,0,0);
}

void sys_paint(int x, int y, uint64_t blue ,uint64_t green, uint64_t red)
{
  _int80(8,(uint64_t)x,(uint64_t)y,blue,green,red);
}

void clear()
{
	_int80(9,0,0,0,0,0);
}

void sys_drawCharPosition(char * c, int x, int y)
{
  _int80(10,(uint64_t) c ,(uint64_t) x ,(uint64_t) y,0,0);
}

int get_screen_info(int op)
{
    return (int)_int80(11, op, 0, 0, 0, 0);
}

void setPointer(int x, int y)
{
  _int80(12, x , y, 0, 0, 0);
}

void color(int blue, int green, int red)
{
	_int80(13, blue , green, red , 0 , 0 );
}

void * allocMemory(uint64_t amount)
{
	return (void *) _int80(14, amount, 0, 0, 0, 0);
}

void freeMemory(void * address)
{
	_int80(15,(uint64_t)address, 0, 0, 0, 0);
}

void end()
{
  _int80(16, 0, 0, 0, 0, 0);
}

void sleep(int milliseconds)
{
	_int80(17, (uint64_t) milliseconds, 0, 0, 0, 0);
}

void yield()
{
  _int80(18, 0, 0, 0, 0, 0 );
}

int kill(int pid)
{
  return (int)_int80(19,(uint64_t) pid, 0, 0, 0, 0);
}

int exec(void * ptr, uint64_t params, const char * name)
{
  return _int80(20, (uint64_t) ptr, params, (uint64_t) name, 0 , 0 );
}

int ppid()
{
  return _int80(21, 0, 0, 0, 0, 0);
}

int pid()
{
  return _int80(22, 0, 0, 0, 0, 0);
}

int openPipe(char * name)
{
	int fd;
	_int80(23,(uint64_t ) name, (uint64_t)&fd, 0, 0, 0);
	return fd;
}

void closePipe(char * name)
{
	_int80(24,(uint64_t) name, 0, 0, 0, 0);
}

int semaphore_signal(int key)
{
  return _int80(25,(uint64_t)key, 0, 0, 0 , 0);
}

int semaphore_wait(int semaphore_key, int lock_key)
{
  return _int80(26,(uint64_t) semaphore_key,(uint64_t) lock_key, 0,0,0);
}

int semaphore_open(char * name)
{
  return _int80(27, (uint64_t) name, 0, 0, 0, 0);
}

int semaphore_broadcast(int key)
{
  return _int80(28,(uint64_t)key, 0, 0, 0, 0);
}

int mutex_lock(int key)
{
  return _int80(29, (uint64_t) key, 0, 0, 0, 0);
}

int mutex_unlock(int key)
{
  return _int80(30, (uint64_t) key, 0, 0, 0, 0);
}

int mutex_open(char * name)
{
  return _int80(31, (uint64_t) name, 0, 0, 0, 0);
}

int mutex_close(int key)
{
  return _int80(32, (uint64_t) key, 0, 0, 0, 0);
}

int get_current_pids(int * pid_array)
{
  return _int80(33, (uint64_t) pid_array, 0, 0, 0, 0);
}

int set_foreground(int pid)
{
  return (int)_int80(34, (uint64_t) pid, 0, 0, 0, 0);
}

int get_process_info(int pid, struct process_info_c * pi)
{
  return (int)_int80(35,(uint64_t) pid, (uint64_t) pi, 0, 0, 0);
}

int get_semaphore_info(semaphore_info info_array[])
{
  return _int80(36, (uint64_t) info_array, 0, 0, 0, 0);
}

int get_mutexes_info(mutex_info info_array[])
{
  return _int80(37, (uint64_t) info_array, 0, 0, 0 , 0);
}

int semaphore_close(int key)
{
  return _int80(38, key, 0, 0, 0, 0);
}

char * cd( char * pathName)
{
  return (char *)_int80(38,(uint64_t) pathName, 0, 0, 0, 0);
}

int sys_createFile(char * path, char * fileName, uint64_t isDir)
{
  return _int80(40, (uint64_t) path, (uint64_t) fileName, isDir, 0, 0);

}

int sys_openFile(char * path, char * name ,uint64_t state)
{
  return _int80(41, (uint64_t)path, (uint64_t) name, state, 0, 0);

}

int sys_closeFile(char * path, char * name)
{
  return _int80(42, (uint64_t)path,(uint64_t) name, 0, 0, 0);
}


int sys_readFile(char * path, char * name, uint64_t index)
{
  return _int80(43, (uint64_t) path, (uint64_t) name, index, 0, 0);

}

int sys_writeFile(char * path, char * name, char * bytes, uint64_t count)
{
  return _int80(44, (uint64_t)path,(uint64_t) name, (uint64_t) bytes, count, 0);

}

int sys_removeFile(char * path, char * name)
{
  return _int80(45, (uint64_t)path, (uint64_t) name, 0, 0, 0);

}

int sys_getFileInfo(char * path, struct file_info_c * fi)
{
  return _int80(46, (uint64_t) path, (uint64_t)fi, 0, 0, 0);

}
