#ifndef FILE_H
#define FILE_H


#define MAX_FILE_NAME 20
#define MAX_SONS 16
#define MAX_FILES 128
#define HOME "home"

typedef struct file_t{
	char name[MAX_FILE_NAME];
  char isDir;
  file * sons[MAX_SONS];
	uint64_t sonsAmount;
  file * father;
	//faltaria MEMORIA.
} file;


#endif
