#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <systemCallDispatcher.h>
#include <interrupts.h>
#include <videoDriver.h>
#include <dirs.h>
#include <semaphore.h>
#include <buddyAllocator.h>
#include <pipe.h>
#include <mutex.h>
#include <scheduler.h>
#include <process.h>
#include <init.h>
#include <idtLoader.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)CODE_ADDRESS;
static void * const sampleDataModuleAddress = (void*)DATA_ADDRESS;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	_cli();
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	return getStackBase();
}


int main()
{
	load_idt();
	//((EntryPoint)sampleCodeModuleAddress)();
	draw_word("IDT cargada \n");
	initializeHeap();
	draw_word("Heap iniciado Deberia iniciarse metodo init \n");
	
	sys_exec((uint64_t)init, 0,"init");
	draw_word("Esto no deberia IMPRIMIRSE\n");
	while(1);

	// printAllProcesses();
	//void ** pargs= (void**)buddyAllocate(sizeof(void*));
	//pargs[0] = (void*)"init";
	//insertProcess(&init, 1, pargs);
	//pargs[0] = (void*)"shell";
	// insertProcess(sampleCodeModuleAddress, 1, pargs);
	// setForeground(1);
 	// beginScheduler();

	return 0;
}
