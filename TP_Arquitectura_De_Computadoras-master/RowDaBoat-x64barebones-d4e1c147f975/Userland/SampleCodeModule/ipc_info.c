#include "ipc_info.h"
#include "syscalls.h"
#include "stdio.h"

void print_ipc_info() {
	semaphore_info c_info[MAX_SEMAPHORE];
	mutex_info m_info[MAX_MUTEXES];

	int c_count = get_semaphore_info(c_info);
	int m_count = get_mutexes_info(m_info);

	print_conds_info(c_info, c_count);
	print_mutexes_info(m_info, m_count);
}

void print_conds_info(cond_info info_array[], int count) {
	int i;
	printf(" - Open Condition Variables -\n");
	for (i = 0; i < count; i++) {
		printf("%s - Key: %d\n", info_array[i].name, info_array[i].key);
	}
	putchar('\n');
}

void print_mutexes_info(mutex_info info_array[], int count) {
	int i;
	printf(" - Open Mutexes -\n");
	for (i = 0; i < count; i++) {
		printf("%s - Key: %d - State: %s\n", info_array[i].name, info_array[i].key, \
			info_array[i].locked ? "UNLOCKED" : "LOCKED");
	}
	putchar('\n');
}

void print_fifos_info(fifo_info info_array[], int count) {
	int i;
	printf(" - Open FIFOs -\n");
	for (i = 0; i < count; i++) {
		printf("%s - File Descriptor: %d\n", info_array[i].name, info_array[i].fds);
	}
	putchar('\n');
}
