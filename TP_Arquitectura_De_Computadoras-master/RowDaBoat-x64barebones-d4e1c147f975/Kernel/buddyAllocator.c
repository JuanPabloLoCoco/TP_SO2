#include <buddyAllocator.h>
#include <videoDriver.h>
#include <lib.h>
#include <mutex.h>
#include <scheduler.h>
#include <math.h>
#include <defs.h>
#include <dirs.h>
#include <process.h>

#define MEMBEGIN 0x10000000
#define MAXMEMORY (128*1024*1024)
#define MINPAGE (4*1024)
#define PAGE_SIZE 4*1024
#define MAXHEAPSIZE (MAXMEMORY/MINPAGE)*2-1
#define MAX_PROCESSES 128

#define PARENT(i) ((i) >> 1)
#define LCHILD(i) ((i) << 1)
#define RCHILD(i) (((i) << 1) + 1)
#define AMILEFT(i) !((i) % 2)
#define ISNAVAILABLE(i,n) ((i)&(myBit(n)))

static uint64_t stack_pages_stack[ MAX_PROCESSES ];
static uint16_t current_stack_index = 0; /* Apunta al elemento a poppear */

static uint16_t heap[MAXHEAPSIZE];
static uint64_t heapSize = 0;
static uint64_t block = 0;
static void* beginning = (void*) MEMBEGIN;
static int mutex = 0;
static uint64_t stack_memory_mutex_key = -1;

void* buddyAllocate(uint64_t amount)
{
    if(amount<=0) return NULL;

    mutex_lock(mutex);

    uint64_t pages = roundUpPower2(amount/block);
    pages = pages == 0 ? 1 : pages;
    void* ans = addNblocks(pages);

    mutex_unlock(mutex);

    return ans;
}

void* buddyAllocatePages(uint64_t pages)
{
    if (pages == 0) return NULL;

    mutex_lock(mutex);

    void* ans = addNblocks(pages);

    mutex_unlock(mutex);

    return ans;
}


uint16_t myBit(uint16_t n)
{
    int i = 0;
    while(n)
    {
        n = n >> 1;
        i++;
    }
    return 1 << (i - 1);
}

uint16_t myMask(uint16_t n)
{
    uint16_t i = 0;
    while(n)
    {
        n--;
        i = (i << 1) + 1;
    }
    return i;
}

void initializeHeap()
{
    block = MINPAGE;
    heapSize = MAXHEAPSIZE;
    recursiveMark(1);
}

void initializeHeapMutex()
{
  mutex = mutex_open(PAGESMUTEX);
}

void initialize_stack_memory_allocator()
{
	int i;
	for (i = 0; i < MAX_PROCESSES; i++){
		stack_pages_stack[i] = (i+1) * STACK_PAGE_SIZE + STACK_ADDRESS;
  }
    /* Se suma 1 porque el stack crece
		** hacia direcciones menores */
}

void initialize_memory_allocator_mutex()
{
	stack_memory_mutex_key = mutex_open("__STACK_MEMORY_MUTEX__");
}

uint16_t recursiveMark(int index)
{
    if(index > heapSize/2)
    {
        heap[index] = 1;
        return (1 << 1) + 1;
    }

    recursiveMark(RCHILD(index));
    uint16_t mark = recursiveMark(LCHILD(index));
    heap[index] = mark;
    return (mark << 1) + 1;
}

/* Se ignora el valor de size. Se asume que será menor a PAGE_SIZE.
** Se hizo así para que la interfaz tenga sentido, en un futuro puede
** cambiar la implementación a una que si contemple el size correctamente.
*/
uint64_t get_stack_page(uint64_t size)
{
  mutex_lock(stack_memory_mutex_key);
	if (current_stack_index < MAX_PROCESSES)
  {
		uint64_t page = stack_pages_stack[current_stack_index++];
		mutex_unlock(stack_memory_mutex_key);
		return page;
	}
	mutex_unlock(stack_memory_mutex_key);
	return 0;

}

uint64_t store_stack_page(uint64_t address)
{
	mutex_lock(stack_memory_mutex_key);
	if (current_stack_index > 0)
  {
		stack_pages_stack[--current_stack_index] = address;
		mutex_unlock(stack_memory_mutex_key);
		return 1;
	}
	mutex_unlock(stack_memory_mutex_key);
	return 0;
}


void* addNblocks(unsigned char  n)
{
    int a = ((1 + heapSize)/2);

    if(n > ((1 + heapSize)/2)) return NULL;

    return recursiveAdd(1, n, (uint64_t)beginning, MAXMEMORY);
}

void* recursiveAdd(int i, uint16_t n, uint64_t address, uint64_t innerSize)
{
    void*ans = NULL;
    if(!ISNAVAILABLE(heap[i], n))
    {
        return NULL;
    }
    else
    {
        if(i <= heapSize/2 && ISNAVAILABLE(heap[RCHILD(i)], n))
        {
            ans= recursiveAdd(RCHILD(i), n, address + (innerSize/2), innerSize/2);
            heap[i] = heap[RCHILD(i)] | heap[LCHILD(i)];
        }
        else if(i <= heapSize/2 && ISNAVAILABLE(heap[LCHILD(i)], n))
        {
            ans = recursiveAdd(LCHILD(i),n,address,innerSize/2);
            heap[i] = heap[RCHILD(i)] | heap[LCHILD(i)];
        }
        else
        {
            heap[i] = 0;
            return (void*)address;
        }
    }
    return ans;
}

int isInt(float f)
{
    return (f-(int)f == 0);
}

int buddyFree(void* address)
{
    int ans;

    mutex_lock(mutex);

    address = (void*)((char*)address - (char*)beginning);
    if(!isInt((uint64_t)address/(block*1.0f)))
    {
        ans = -1;
    }
    else
    {
        int position = ((uint64_t)address)/block;
        ans = searchUp(heapSize/2 + 1 + position,1);
    }

    mutex_unlock(mutex);

    return ans;
}


int searchMemoryUp(int i, uint16_t level)
{
    if(i < 1) return 0;
    if(heap[i] == 0)
    {
        return pow2(level-1);
    }
    else if(AMILEFT(i))
    {
        return searchMemoryUp(PARENT(i), level+1);
    }
    else
    {
        return 0;
    }
}


int searchUp(int i, uint16_t level)
{
    if(i < 1) return -1;
    if(heap[i] == 0)
    {
        heap[i] = myMask(level);
        releaseUp(PARENT(i), level+1);
        return 0;
    }
    else if(AMILEFT(i))
    {
        return searchUp(PARENT(i), level+1);
    }
    else
    {
        return -1;
    }
}

void releaseUp(int i,uint16_t level)
{
    if(i < 1) return;
    if(heap[RCHILD(i)] == myMask(level-1) && heap[LCHILD(i)] == myMask(level-1))
    {
        heap[i] = myMask(level);
    }
    else
    {
        heap[i] = heap[RCHILD(i)] | heap[LCHILD(i)];
    }
    releaseUp(PARENT(i), level+1);
}
