#ifndef SYSTEM_CALLER_DISPATCHER_H
#define SYSTEM_CALLER_DISPATCHER_H

#define INIT 0
#define SHELL 1

#include <ipc_info.h>
#include <file.h>


uint64_t sys_read(uint64_t fd, uint64_t destination, uint64_t count);
uint64_t wrap_sys_read(uint64_t fd, char* destination, uint64_t count);
uint64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t count);
uint64_t wrap_sys_write(uint64_t fd, const char* buffer, uint64_t count);
uint8_t sys_seconds(void);
uint8_t sys_mins(void);
uint8_t sys_hours(void);
uint32_t sys_year(void);
uint8_t sys_month(void);
uint8_t sys_day(void);
uint8_t getSeconds(void);
uint8_t getMinutes(void);
uint8_t getHours(void);
uint32_t getYear(void);
uint8_t getMonth(void);
uint8_t getDay(void);
uint64_t sys_paint(uint64_t x, uint64_t y);
uint64_t sys_clear(void);
uint64_t sys_drawCharPosition(uint64_t l,uint64_t x, uint64_t y);
uint64_t sys_getScreenInfo(uint64_t rdi);
uint64_t sys_setPointer(uint64_t x, uint64_t y);
uint64_t sys_ChangeColor(uint64_t blue,uint64_t green,uint64_t red);

uint64_t sys_allocPage(uint64_t toAlloc);
uint64_t sys_free(uint64_t toFree);

uint64_t sys_end();
// uint64_t sys_ps(uint64_t buffer, uint64_t size);

uint64_t sys_exec(uint64_t ptr, uint64_t params,const char * name);

uint64_t sys_ppid();
uint64_t sys_pid();

uint64_t sys_get_pid(uint64_t pid_array);
uint64_t sys_set_foreground(uint64_t pid);
uint64_t sys_kill(uint64_t pid);
uint64_t sys_end();
//uint64_t sys_process_info(uint64_t pid, struct process_info_c * pi);

uint64_t sys_sleep(uint64_t milliseconds);
uint64_t sys_yield();

uint64_t sys_openPipe(uint64_t name,uint64_t ans);
uint64_t sys_closePipe(uint64_t name);

uint64_t sys_waitSemaphore(uint64_t semaphore_key, uint64_t lock_key);
uint64_t sys_signalSemaphore(uint64_t key);
uint64_t sys_openSemaphore(uint64_t name);
uint64_t sys_broadcastSemaphore(uint64_t key);
uint64_t sys_closeSemaphore(uint64_t key);
uint64_t sys_get_semaphore_info(semaphore_info info_array[]);

uint64_t sys_openMutex(uint64_t name);
uint64_t sys_closeMutex(uint64_t key);
uint64_t sys_lockMutex(uint64_t key);
uint64_t sys_unlockMutex(uint64_t key);
uint64_t sys_get_mutexes_info(mutex_info * info_array);

uint64_t sys_cd_wr(char * fileName, char * resp);
uint64_t sys_cd(uint64_t fileName, uint64_t resp);
uint64_t sys_mkdir(uint64_t name);
uint64_t sys_openFile(uint64_t path, uint64_t name ,uint64_t state);

uint64_t sys_createFile(uint64_t path, uint64_t name, uint64_t isDir);
uint64_t sys_closeFile(uint64_t path, uint64_t name);
uint64_t sys_writeFile(uint64_t path, uint64_t name, uint64_t bytes, uint64_t count);
uint64_t sys_readFile(uint64_t path, uint64_t name, uint64_t index);
uint64_t sys_getFileInfo(uint64_t * path, uint64_t fi);
uint64_t sys_removeFile(uint64_t path, uint64_t name);

#endif
