#ifndef FILE_H
#define FILE_H

#define MAX_FILE_NAME 20
#define MAX_SONS 16
#define MAX_FILES 128
#define HOME "home"
#define FILESYSTEMMUTEX "_FS_MUTEX_"
#define BLOCKSIZE 4 * 1024
#define BLOCKCOUNT 256

#define DIR 0;
#define NOT_DIR 1;

#define CLOSE 0
#define READ 1
#define WRITE 2

#define OPEN_IN_OTHER_MODE_ERR 4
#define NOTAFILE -1
#define OPEN_IN_OTHER_PROCESS_ERR 5
#define NO_MORE_CHILD_ERR 2

typedef struct file_t{
	char name[MAX_FILE_NAME];
  uint64_t isDir;
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

void initFileSystem();

file * changeDirectory(char * name);

file * createFile(char * name , file * father, uint64_t isDir);
uint64_t addSon(file * father, file * son);

file * createDir(char * name, file * father);
file * createNotDir(char * name, file * father);
uint64_t deleteFile(file * fileToDelete);
void deleteMyFatherReference(file * father, file * son);


fileBlock * getBlock();
void realeseBlock(fileBlock * block);
void realeseFreeBlocks(File * file);
void realseAllBlocks(file * fileToDelete);
void initializeFileBlocks();

uint64_t writeOnFile(file * thisFile, void * bytes, uint64_t count);

void * readFile(file * file, uint64_t index);

uint64_t openFile(file * thisFile, uint64_t state);
uint64_t closeFile(file * thisFile);

file * getCurrentDir();
file * getHome();
char * getName(file * f);
char isDir(file * f);
char * pathName(file * f);
file * pathToFile(char * pathName);
file * fileChild(file * thisFile,char * bufferDir);
file * getFileFromPath(char * pathName, char * name);

#endif
