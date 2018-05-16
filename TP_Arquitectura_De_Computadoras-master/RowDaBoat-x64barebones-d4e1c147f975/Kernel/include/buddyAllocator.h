#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H

#include "lib.h"

#define MEMBEGIN 0x10000000
#define MAXMEMORY (128*1024*1024) //in b//
#define MINPAGE (4*1024) //in b//
#define PAGE_SIZE 4*1024

void initializeHeap();
void initializeHeapMutex();
void* buddyAllocatePages(uint64_t pages);
void* buddyAllocate(uint64_t amount);
uint16_t myBit(uint16_t n);
uint16_t myMask(uint16_t n);

uint16_t recursiveMark(int index);
void* addNblocks(uint8_t  n);
void* recursiveAdd(int i, uint16_t n, uint64_t address, uint64_t innerSize);
void releaseUp(int i, uint16_t level);

int isInt(float f);
int buddyFree(void* address);
int searchMemoryUp(int i, uint16_t level);
int searchUp(int i, uint16_t level);
void releaseUp(int i,uint16_t level);




#endif
