#ifndef FILE_H
#define FILE_H

#define MAX_FILE_NAME 20
#define MAX_SONS 16
#define MAX_FILES 128
#define HOME "home"
#define FILESYSTEMMUTEX "_FS_MUTEX_"
#define BLOCKSIZE 4 * 1024
#define BLOCKCOUNT 256

#define CLOSE 0
#define READ 1
#define WRITE 2

#define OPEN_IN_OTHER_MODE_ERR 4
#define NOTAFILE -1
#define OPEN_IN_OTHER_PROCESS_ERR 5

typedef struct file_t{
	char name[MAX_FILE_NAME];
  char isDir;
  struct file_t * sons[MAX_SONS];
	uint64_t sonsAmount;
  struct file_t * father;
	fileBlock * blocks;
	uint64_t index;
	uint64_t state;

	process * curr;
} file;
//faltaria MEMORIA.


typedef struct fileBlock{
  void * adress;
  FileBlock * next;
  uint64_t size;
};


file * surfDirectory(char * name);
file * createDir(char * name , file * father);
char * pathName(file * f);
char isDir(file * f);
char * getName(file * f);
char * getSonsName(file * f);
void initFileSystem();

fileBlock * getBlock();
void realeseBlock(fileBlock * block);
void initializeFileBlocks();
uint64_t writeOnFile(file * thisfile, void * bytes, uint64_t count);
void * readFile(file * file, uint64_t index);




#endif
