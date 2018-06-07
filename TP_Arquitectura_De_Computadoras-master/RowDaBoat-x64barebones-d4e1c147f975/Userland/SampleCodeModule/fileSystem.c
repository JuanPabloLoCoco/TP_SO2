#include <fileSystem.h>
#include <file_info.h>
#include <syscalls.h>
#include <stdlib.h>

char * getCurrentDir()
{
    return cd(".");
}

void listDirectory()
{
  char * path = getCurrentDir();
  file_info * fi =(file_info *) malloc(sizeof(file_info));

  if (sys_getFileInfo(path, fi))
	{
    printf("%s\n", fi->name);
    for (int x = 0; x < fi->sonsAmount ;x++)
    {
      printf("   ");
      printf("%s\n", fi->sons[x]);
    }
  }
}



openFile( char * path, char * name, uint64_t state)
{

}

closeFile(char * path, char * name)
{

}

writeFile(char * path, char * name, char *bytes, uint64_t count)
{

}

readFile(char * path, char * name, uint64_t index)
{

}
