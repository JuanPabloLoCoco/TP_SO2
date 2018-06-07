#include <systemCallDispatcher.h>
#include <stdint.h>
#include <videoDriver.h>
#include <buddyAllocator.h>
#include <scheduler.h>
#include <time.h>
#include <semaphore.h>
#include <mutex.h>
#include <process.h>
#include <defs.h>
#include <ipc_info.h>
#include <process_info.h>
#include <file.h>
#include <file_info.h>

int getChar(void);
void _sti(void);
static uint64_t sys_exec_wr(uint64_t ptr, uint64_t params, uint64_t name);
static uint64_t sys_get_mutexes_info_wr(uint64_t info_array);
static uint64_t sys_process_info_wr (uint64_t pid, uint64_t pi);
static uint64_t sys_get_semaphore_info_wr(uint64_t info_array);

uint64_t systemCallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r8, uint64_t r9)
{
		uint64_t result;
		switch(rax)
		{
			case 0:
				result = sys_read(rdi, (char *)rsi, rdx);
				break;
			case 1:
				 result = sys_write(rdi, (char *)rsi, rdx);
				 break;
			case 2:
				result = (uint64_t)sys_seconds();
				break;
			case 3:
				result= (uint64_t)sys_mins();
				break;
			case 4:
				result = (uint64_t)sys_hours();
				break;
			case 5:
				result = sys_year();
				break;
			case 6:
				result = sys_month();
				break;
			case 7:
				result = sys_day();
				break;
			case 8:
				result = sys_paint(rdi, rsi);
				break;
			case 9:
				result = sys_clear();
				break;
			case 10:
				result = sys_drawCharPosition(rdi, rsi, rdx);
				break;
			case 11:
				result = sys_getScreenInfo(rdi);
				break;
			case 12:
				result = sys_setPointer(rdi, rsi);
				break;
			case 13:
				result = sys_ChangeColor(rdi, rsi, rdx);
				break;
			case 14:
				result = sys_allocPage(rdi);
				break;
			case 15:
				result = sys_free(rdi);
				break;
			case 16:
				result = sys_end();
				break;
			case 17:
				result = sys_sleep(rdi);
				break;
			case 18:
				result = sys_yield();
				break;
			case 19:
				result = sys_kill(rdi);
				break;
			case 20:
				result = sys_exec_wr(rdi,rsi,rdx);
				break;
			case 21:
				result = sys_ppid();
				break;
			case 22:
				result = sys_pid();
				break;
			case 23:
				result = sys_openPipe(rdi,rsi);
				break;
			case 24:
				result = sys_closePipe(rdi);
				break;
			case 25:
				result = sys_signalSemaphore(rdi);
				break;
			case 26:
				result = sys_waitSemaphore(rdi,rsi);
				break;
			case 27:
				result = sys_openSemaphore(rdi);
				break;
			case 28:
				result = sys_broadcastSemaphore(rdi);
				break;
			case 29:
				result = sys_lockMutex(rdi);
				break;
			case 30:
				result = sys_unlockMutex(rdi);
				break;
			case 31:
				result = sys_openMutex(rdi);
				break;
			case 32:
				result = sys_closeMutex(rdi);
				break;
			case 33:
				result = sys_get_pid(rdi);
				break;
			case 34:
				result = sys_set_foreground(rdi);
				break;
			case 35:
				result = sys_process_info_wr(rdi,rsi);
				break;
			case 36:
				result = sys_get_semaphore_info_wr(rsi);
				break;
			case 37:
				result = sys_get_mutexes_info_wr(rsi);
				break;
			case 38:
				result = sys_closeSemaphore(rsi);
				break;
			case 39:
				result = sys_cd(rsi);
				break;
			case 40:
				result = sys_createFile(rsi, rdi, rdx);
				break;
			case 41:
				result = sys_openFile(rsi,rdi,rdx);
				break;
			case 42:
				result = sys_closeFile(rsi, rdi);
				break;
			case 43:
				result = sys_readFile(rsi, rdi, rdx);
				break;
			case 44:
				result = sys_writeFile(rsi,rdi,rdx,r8);
				break;
			case 45:
				result = sys_removeFile(rsi, rdi);
				break;
			case 46:
				result = sys_getFileInfo(rsi, rdi);
				break;
		}
		return result;
}

uint64_t sys_read(uint64_t fd, uint64_t destination, uint64_t count)
{
	return wrap_sys_read(fd, (char*) destination, count);
}

uint64_t wrap_sys_read(uint64_t fd, char* destination, uint64_t count)
{
	uint64_t i = 0;
	char c = 0;
  if (fd == 0)
	{
		if (get_current_process() != get_foreground_process())
		{
			block_foreground_process(get_current_process());
			yield_process();
		}
		while (i < count)
		{
			c = getChar();
			if(c != -1)
			{
				destination[i] = c;
				i++;
			}
			else
			{
				_sti();
				block_read_process(get_current_process());
				yield_process();
			}
		}
  }
	else if (fd > 2 && fd < 8)
	{
			pipe_t pipe = (get_current_process())->fd[fd-3];
			readPipe(pipe, destination, count);
	}
  return i;
}

uint64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t count)
{
	return wrap_sys_write(fd, (const char*) buffer, count);
}

uint64_t wrap_sys_write(uint64_t fd, const char* buffer, uint64_t count)
{
	int i = 0;
	if(fd == 1)
	{
		while(i < count)
		{
			if(buffer[i] == '\n')
				newLine();
			else
				draw_char(buffer[i]);
			i++;
		}
	}
	else if (fd > 2 && fd < 8)
	{
		pipe_t pipe = (get_current_process())->fd[fd-3];
		writePipe(pipe, buffer, count);
	}
	return i;
}

uint8_t sys_seconds()
{
	uint8_t seconds = getSeconds();
	return seconds;
}


uint8_t sys_mins()
{
	uint8_t minutes = getMinutes();
	return minutes;
}

uint8_t sys_hours()
{
	uint8_t hours = getHours();
	return hours;
}


uint32_t sys_year()
{
	uint32_t year = getYear();
	return year;
}


uint8_t sys_month()
{
	uint8_t month = getMonth();
	return month;
}

uint8_t sys_day()
{
	uint8_t day = getDay();
	return day;
}


uint64_t sys_paint(uint64_t x, uint64_t y)
{
	draw_pixel(x,y);
	return 0;
}

uint64_t sys_clear()
{
	cls();
	return 0;
}


uint64_t sys_drawCharPosition(uint64_t l, uint64_t x, uint64_t y)
{
	draw_char_position(l, x, y);
	return 0;
}

uint64_t sys_getScreenInfo(uint64_t rdi)
{
	switch(rdi)
	{
		case 0:
			return getScreenWidth();
		case 1:
			return getScreenHeigth();
	}
	return 0;
}

uint64_t sys_setPointer(uint64_t x, uint64_t y)
{
	setPointer(x,y);
	return 0;
}

uint64_t sys_ChangeColor(uint64_t blue,uint64_t green,uint64_t red)
{
	setFontColor(blue,green,red);
	return 0;
}

/*------------------------Memory Allocation----------------------*/
uint64_t sys_allocPage(uint64_t toAlloc)
{
 	void* address = buddyAllocate(toAlloc);
	return (uint64_t) address;
}

uint64_t sys_free(uint64_t toFree)
{
 	buddyFree((void *) toFree);
	return 0;
}

/*------------------------Process & scheduler----------------------*/
static uint64_t sys_exec_wr(uint64_t entry_point, uint64_t params,uint64_t name)
{
    return sys_exec(entry_point,params,(const char *)name);
}

uint64_t sys_exec(uint64_t ptr, uint64_t params,const char * name)
{
	return exec(createProcess(ptr,params,name));
}

uint64_t sys_ppid()
{
	return ppid_process(get_current_process());
}

uint64_t sys_pid()
{
    return pid_process(get_current_process());
}

uint64_t sys_get_pid(uint64_t pid_array)
{
	return get_current_pids((int *) pid_array);
}

uint64_t sys_set_foreground(uint64_t pid)
{
	process * p = get_process_by_pid(pid);
	if (p == NULL)
	{
		return 0;
	}
	set_foreground_process(p);
	return 1;
}

uint64_t sys_kill(uint64_t pid)
{
	int valid = 0;
	if (pid != INIT && pid != SHELL)
	{
		process * p = get_process_by_pid(pid);
		valid = kill_process(p);
	}
	return valid;
}

uint64_t sys_end()
{
	end_process();
	return 0;
}

uint64_t sys_sleep(uint64_t milliseconds)
{
  return sleep(milliseconds);
}

uint64_t sys_yield()
{
    yield_process();
    return 1;
}

uint64_t sys_process_info(uint64_t pid, struct process_info_c * pi)
{
	return get_process_info_by_pid(pi, pid);
}

static uint64_t sys_process_info_wr(uint64_t pid, uint64_t pi)
{
	return sys_process_info(pid,(struct process_info_c *) pi);
}


/*------------------------PIPES ----------------------*/

uint64_t sys_openPipe(uint64_t name, uint64_t ans) {
  	int * a = (int *) ans;
  	*a = addPipe(getPipe((char *)name)) + 3;
  	return 0;
}

uint64_t sys_closePipe(uint64_t name) {
    releasePipe((char *)name);
    return 0;
}

/*------------------------SEMAPHORES ----------------------*/

uint64_t sys_waitSemaphore(uint64_t semaphore_key, uint64_t lock_key)
{
		return semaphore_wait(semaphore_key,lock_key);
}

uint64_t sys_signalSemaphore(uint64_t key)
{
    return semaphore_signal(key);
}

uint64_t sys_openSemaphore(uint64_t name)
{
		return semaphore_open((char *)name);
}

uint64_t sys_broadcastSemaphore(uint64_t key)
{
    return semaphore_broadcast(key);
}

static uint64_t sys_get_semaphore_info_wr(uint64_t info_array)
{
	return sys_get_semaphore_info((semaphore_info *) info_array);
}

uint64_t sys_get_semaphore_info(semaphore_info info_array[])
{
	return get_semaphore_info(info_array);
}

uint64_t sys_closeSemaphore(uint64_t key)
{
	return semaphore_close((int) key);
}

/*------------------------MUTEX ----------------------*/
uint64_t sys_openMutex(uint64_t name)
{
    return mutex_open((char *)name);
}

uint64_t sys_closeMutex(uint64_t key)
{
    return mutex_close(key);
}


uint64_t sys_lockMutex(uint64_t key)
{
    return mutex_lock(key);
}

uint64_t sys_unlockMutex(uint64_t key)
{
    return mutex_unlock(key);
}

static uint64_t sys_get_mutexes_info_wr(uint64_t info_array)
{
	return sys_get_mutexes_info((mutex_info *) info_array);
}

uint64_t sys_get_mutexes_info(mutex_info * info_array)
{
	return get_mutexes_info(info_array);
}

/*------------------------DIRECTORY_SYSTEM ----------------------*/
uint64_t sys_cd_wr(char * fileName)
{
	file * resp = changeDirectory(fileName);
	if (resp == NULL)
	{
		return "/home";
	}
	char ans[128];
	return (uint64_t) pathName(resp, ans);
}

uint64_t sys_cd(uint64_t fileName)
{
	return sys_cd_wr((char *) fileName);
}

uint64_t sys_createFile(uint64_t path, uint64_t name, uint64_t isDir)
{
	file * father = pathToFile((char *) path);
	if (father == NULL)
	{
		return (uint64_t) NULL;
	}

	file * resp = fileChild(father,(char *) name);

	if (resp == NULL || strcmp(father->name,(char *) name) == 0)
	{
		return (uint64_t)	createFile((char *) name, father, isDir);
	}
	else
	{
		return resp;
	}
}

uint64_t sys_openFile(uint64_t path, uint64_t name ,uint64_t state)
{
	file * fileToOPen = (file *) sys_createFile(path, name, NOT_DIR);
	if (fileToOPen == NULL)
	{
		return NOTAFILE;
	}
	return openFile(fileToOPen, state);
}

uint64_t sys_closeFile(uint64_t path, uint64_t name)
{
	file * resp = getFileFromPath((char *) path, (char *) name);
	return closeFile(resp);
}

uint64_t sys_writeFile(uint64_t path, uint64_t name, uint64_t bytes, uint64_t count)
{
	file * resp = getFileFromPath((char *) path, (char *) name);
	if (resp == NULL)
	{
		return -1;
	}
	return writeOnFile(resp, (void *)bytes, count);
}

uint64_t sys_readFile(uint64_t path, uint64_t name, uint64_t index)
{
	file * resp = getFileFromPath((char *) path, (char *) name);
	if (resp == NULL)
	{
		return -1;
	}
	return (uint64_t)readFile(resp, index);
}

uint64_t sys_getFileInfo(uint64_t * path, uint64_t fi)
{
	file * thisFile = pathToFile((char *) path);
	return get_file_info((struct file_info_c *) (fi), thisFile);
}

uint64_t sys_removeFile(uint64_t path, uint64_t name)
{
	file * resp = getFileFromPath((char *) path, (char *) name);
	if (resp == NULL)
	{
		return -1;
	}
	return (uint64_t)deleteFile(resp);

}
