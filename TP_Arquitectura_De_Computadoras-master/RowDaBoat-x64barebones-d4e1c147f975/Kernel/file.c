#include "include/lib.h"
#include "include/buddyAllocator.h"
#include <file.h>
#include <defs.h>
#include "include/strings.h"

static file * openFiles[MAX_FILES];
static file * currentDir = NULL;
static file * home = NULL;

void initFileSystem()
{
  //Inicializar MEMORIA
  currentDir = createDir(HOME, NULL);
  home = currentDir;
}

char * getSonsName(file * f)
{
  uint64_t size = f->sonsAmount;
  char * resp = buddyAllocate (MAX_SONS * (MAX_FILE_NAME + 5));
  for (int p = 0; p < size ; p++)
  {
      // strcat(resp, f->sons[p]->name);
      // strcat(resp, "  /  ");
  }
  return resp;
}

char * getName(file * f)
{
  return f->name;
}

char isDir(file * f)
{
  return f->isDir;
}

char * pathName(file * f)
{
  char resp [128];
  if (f == home)
  {
    strcpyWithNoIndex(resp,"/home");
    return resp;
  }
  strcpyWithNoIndex(resp, pathName(f->father));
  // strcat(resp, f->name);
  return resp;
}

file * createDir(char * name , file * father)
{
  if (nameDoesExistInCurrent(name) != 1)
  {
    file * new_file = (file *) buddyAllocate( sizeof(file));
    strcpy( new_file->name, name, MAX_FILE_NAME);
    new_file->father = father;
    new_file->sonsAmount = 0;
    new_file->isDir = 0;
    return new_file;
  }
}

file * surfDirectory(char * name)
{
  if (currentDir == NULL)
  {
    return NULL;

  }

  if (strcmp(name, "..") == 0)
  {
    if (currentDir != home)
    {
      currentDir = currentDir->father;
    }
    return currentDir;
  }

  uint64_t flag = TRUE;
  for(uint64_t z = 0; z < currentDir->sonsAmount && flag ; z++)
  {
    if (strcmp(name, currentDir->sons[z]->name) == 0 )
    {
      flag = FALSE;
      currentDir = currentDir->sons[z];
    }
  }

  return currentDir;
}

char nameDoesExistInCurrent(char * name)
{
  for (int i = 0; i < currentDir->sonsAmount; i++) {
    if (strcmp(name, currentDir->sons[i]->name) == 0)
    {
      return 1;
    }
  }
  return 0;
}
