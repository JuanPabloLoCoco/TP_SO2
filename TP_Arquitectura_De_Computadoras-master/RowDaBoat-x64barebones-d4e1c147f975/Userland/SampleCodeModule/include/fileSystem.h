#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILE_NAME 20
#define MAX_SONS 16

#define DIR 0
#define NOT_DIR 1

#define FILECLOSE 0
#define FILEREAD 1
#define FILEWRITE 2

#define OPEN_IN_OTHER_MODE_ERR 4
#define NOTAFILE -1
#define OPEN_IN_OTHER_PROCESS_ERR 5
#define NO_MORE_CHILD_ERR 2

char * getCurrentDir();
void listDirectory();


#endif
