#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <fileSystem.h>
#include <stdint.h>

typedef struct file_info_c {

  char name[MAX_FILE_NAME];
	uint64_t isDir;
  char sons[MAX_SONS][MAX_FILE_NAME];
  uint64_t sonsAmount
}file_info;

#endif
