#ifndef FILE_H
#define FILE_H

#define MAX_FILE_NAME 20
#define MAX_SONS 16
#define MAX_FILES 128
#define HOME "home"

typedef struct file_t{
	char name[MAX_FILE_NAME];
  char isDir;
  struct file_t * sons[MAX_SONS];
	uint64_t sonsAmount;
  struct file_t * father;
} file;
//faltaria MEMORIA.

file * surfDirectory(char * name);
file * createDir(char * name , file * father);
char * pathName(file * f);
char isDir(file * f);
char * getName(file * f);
char * getSonsName(file * f);
void initFileSystem();
char nameDoesExistInCurrent(char * name);

#endif
