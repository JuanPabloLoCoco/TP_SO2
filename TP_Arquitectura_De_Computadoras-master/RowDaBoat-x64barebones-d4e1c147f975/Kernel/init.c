#include <init.h>
#include <process.h>
#include <systemCallDispatcher.h>
#include <scheduler.h>
#include <dirs.h>
#include <videoDriver.h>
#include <mutex.h>
#include <interrupts.h>
#include <buddyAllocator.h>
#include <pipe.h>
#include <semaphore.h>
#include <TDDFS.h>

static void * const sampleCodeModuleAddress = (void *) CODE_ADDRESS;

void _hlt();
void _sti();

void init() {
	initialize_semaphore();
	initialize_process_mutex();
	initIPC();
	runFStest();

	_sti();


	sys_exec((uint64_t)sampleCodeModuleAddress, 0, "shell");
	set_foreground_process (get_process_by_pid(1));
	while (1) {
		_hlt();
	}
}
