#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H

#include "lib.h"
#include "dirs.h"

#define MEMBEGIN 0x10000000
#define MAXMEMORY (128*1024*1024) //in b//
#define MINPAGE (4*1024) //in b//
#define PAGE_SIZE 4*1024

#define MAXHEAPSIZE (MAXMEMORY/MINPAGE)*2-1
#define MAXAMOUTBLOCKS (MAXMEMORY/MINPAGE)

#define MAX_PROCESSES 128

#define PARENT(i) ((i) >> 1)
#define LCHILD(i) ((i) << 1)
#define RCHILD(i) (((i) << 1) + 1)
#define AMILEFT(i) !((i) % 2)
#define ISNAVAILABLE(i,n) ((i)&(myBit(n)))


/* Tamaño de stack de nuevos procesos */

#define STACK_PAGE_SIZE 0x100000
#define MEMORY_PAGES_END ((STACK_SIZE + 1)* PAGE_SIZE + DATA_ADDRESS)
#define STACK_ADDRESS (MEMORY_PAGES_END + PAGE_SIZE)

typedef uint8_t page_index_t;
#define STACK_SIZE ((page_index_t) - 1)

void initializeHeap();
void initializeHeapMutex();
void initialize_memory_allocator_mutex();
void initialize_stack_memory_allocator();
uint64_t get_stack_page();
uint64_t store_stack_page(uint64_t address);

void* buddyAllocatePages(uint64_t pages);
void* buddyAllocate(uint64_t amount);
uint16_t myBit(uint16_t n);
uint16_t myMask(uint16_t n);

uint16_t recursiveMark(int index);
void addingRecursiveMark(int i, uint16_t n);
void* addNblocks(uint8_t  n);
void* recursiveAdd(int i, uint16_t n, uint64_t address, uint64_t innerSize);
void releaseUp(int i, uint16_t level);

int isInt(float f);
int buddyFree(void* address);
int searchMemoryUp(int i, uint16_t level);
int searchUp(int i, uint16_t level);
void releaseUp(int i,uint16_t level);

#endif
