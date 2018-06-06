#include "include/lib.h"
#include "include/buddyAllocator.h"
#include <file.h>
#include <defs.h>
#include "include/strings.h"
#include <scheduler.h>
#include <strings.h>
#include <ctype.h>

static file * currentDir = NULL;
static file * home = NULL;
static fileBlock * freeBlocks = NULL;

static int mutex = 0;

static uint64_t writeOnFile_wr(file * thisFile, void * bytes, uint64_t count);
static uint64_t deleteFile_wr(file * fileToDelete);


void initFileSystem()
{
    initializeFileBlocks();
    mutex = mutex_open(FILESYSTEMMUTEX);
    currentDir = createDir(HOME, NULL);
    home = currentDir;
}

file * changeDirectory(char * name)
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

  if (strcmp(name, ".") == 0)
  {
      return currentDir;
  }

  for(uint64_t z = 0; z < currentDir->sonsAmount ; z++)
  {
      if (strcmp(name, currentDir->sons[z]->name) == 0 )
      {
          currentDir = currentDir->sons[z];
          return currentDir;
      }
  }

  return NULL;
}


/*****************FILE_CREATION_&_DELETION*************/

file * createFile(char * name , file * father, uint64_t isDir)
{
    file * new_file = (file *) buddyAllocate( sizeof(file));
    uint64_t resp = addSon(father, new_file);
    if (resp > 0 )
    {
        buddyFree(new_file);
        //deleteFile(new_file);
        return NULL;
    }
    strcpy( new_file->name, name, MAX_FILE_NAME);
    new_file->father = father;
    new_file->sonsAmount = 0;
    new_file->isDir = isDir;
    new_file->blocks = NULL;
    new_file->index = 0;
    new_file->state = CLOSE;
    new_file->curr = NULL;
    return new_file;
}

uint64_t addSon(file * father, file * son)
{
    if (father == NULL || son == NULL)
    {
        return 1;
    }

    if (father->sonsAmount == MAX_SONS)
    {
        return NO_MORE_CHILD_ERR;
    }

    father->sons[father->sonsAmount] = son;
    father->sonsAmount++;
    return 0;
}

file * createDir(char * name, file * father)
{
    return createFile(name, father, DIR);
}

file * createNotDir(char * name, file * father)
{
    return createFile(name, father, NOT_DIR);
}

uint64_t deleteFile(file * fileToDelete)
{
    if (fileToDelete == home)
    {
        return -1;
    }

    uint64_t resp = deleteFile_wr(fileToDelete);

    if (resp == 0)
    {
        deleteMyFatherReference(fileToDelete->father, fileToDelete);
    }
    return resp;
}

static uint64_t deleteFile_wr(file * fileToDelete)
{
    if (fileToDelete->state != CLOSE)
    {
      return -1;
    }

    if (!isDir(fileToDelete))
    {
        realseAllBlocks(fileToDelete);
        return 0;
    }

    uint64_t resp = 0;

    for (uint64_t i = fileToDelete->sonsAmount; i > 0  ; i--)
    {
        resp = deleteFile_wr(fileToDelete->sons[i]);
        if (resp == -1)
        {
            return resp;
        }
    }

    buddyFree(fileToDelete);
    return 0;
}

void deleteMyFatherReference(file * father, file * son)
{
    if (father == NULL || son == NULL)
    {
        return;
    }
    uint64_t i = 0;
    for (uint64_t i = 0; i < father->sonsAmount ; i++)
    {
        if (father->sons[i] == son)
        {
            for(; i < father->sonsAmount - 1; i++)
            {
                father->sons[i] = father->sons[i+1];
            }
            father->sonsAmount--;
            return;
        }
    }
}

/********************* BLOCKS *************/

fileBlock * getBlock()
{
  if (freeBlocks == NULL)
  {
    return NULL;
  }

  fileBlock * ans = freeBlocks;
  freeBlocks = freeBlocks->next;
  ans->next = NULL;
  return ans;
}

void realeseBlock(fileBlock * block)
{
  block->size = 0;
  block->next = freeBlocks;
  freeBlocks = block;
}

void realeseFreeBlocks(file * file)
{
  uint64_t blockIndex = 0;
  fileBlock * currentBlock = file->blocks;

  if (currentBlock == NULL)
  {
    return;
  }

  uint64_t size = file->index / BLOCKSIZE;

  while (size > 0 )
  {
      currentBlock = currentBlock->next;
      size--;
  }
  fileBlock * blocksToDelete  = currentBlock->next;

  currentBlock->next = NULL;

  while (blocksToDelete != NULL)
  {
      fileBlock * aux = blocksToDelete;
      blocksToDelete = blocksToDelete->next;
      realeseBlock(aux);
  }
  return;
}

void realseAllBlocks(file * fileToDelete)
{
    while(fileToDelete->blocks != NULL)
    {
        fileBlock * aux =  fileToDelete->blocks->next;
        realeseBlock(fileToDelete->blocks);
        fileToDelete->blocks = aux;
    }
}

void initializeFileBlocks()
{
  for (uint64_t i = 0; i < BLOCKCOUNT ; i++)
  {
    fileBlock * aux = (fileBlock *) buddyAllocate(sizeof (fileBlock));
    aux->adress = buddyAllocate(BLOCKSIZE-1);
    realeseBlock(aux);
  }
}

/****************write On File *************/

uint64_t writeOnFile(file * thisFile, void * bytes, uint64_t count)
{
    uint64_t resp = writeOnFile_wr(thisFile, bytes, count);
    thisFile->index = count - resp;
    realeseFreeBlocks(thisFile);
}

static uint64_t writeOnFile_wr(file * thisFile, void * bytes, uint64_t count)
{
    if (thisFile->blocks == NULL)
    {
        thisFile->blocks = getBlock();
        if (thisFile->blocks = NULL)
        {
          return count;
        }
    }

    fileBlock * current = thisFile->blocks;

    uint64_t index = 0;

    while (count > 0)
    {
        current->adress[index % BLOCKSIZE] = ((char*)bytes)[index];
        count--;
        index++;

        if (index%BLOCKSIZE == 0 && count > 0)
        {
            if (current->next == NULL)
            {
                current->next = getBlock();
                if ( current->next == NULL)
                {
                    current->adress[(index -1) %BLOCKSIZE] = (char*)EOF;
                    return count - 1;
                }
            }
            current = current->next;
        }

      }

      if (index%BLOCKSIZE == 0)
      {
        if (current->next == NULL)
        {
            current->next = getBlock();
            if ( current->next == NULL)
            {
                current->adress[(index-1)%BLOCKSIZE] = (char*)EOF;
                return count -1;
            }
        }
        current = current->next;
      }

      current->adress[index%BLOCKSIZE] = EOF;
      return count;
}

/******************READ ON FILE ***************/

void * readFile(file * file, uint64_t index)
{
    fileBlock * currentBlock = file->blocks;

    if (index > file->index || index < 0)
    {
        return EOF;
    }

    if (currentBlock == NULL)
    {
        return EOF;
    }

    while (index > BLOCKSIZE -1)
    {
        index = index-BLOCKSIZE;
        currentBlock = currentBlock->next;
        if (currentBlock == NULL)
        {
            return EOF;
        }
    }
    return (void*)(currentBlock->adress[index%BLOCKSIZE]);
}

/******************OPEN FILE && CLOSE FILE************/

//Se supone que llega desde la Syscall con un file.
//Si no existe el file. Lo genera.

uint64_t openFile(file * thisFile, uint64_t state)
{
    mutex_lock(mutex);

    // Puede que nunca entre en esta situacion;

    if (thisFile == NULL)
    {
        return NOTAFILE;
    }

    if (state == 0)
    {
        thisFile->state = state;
        thisFile->curr = get_current_process();
        return state;
    }
    else if (thisFile->state != state)
    {
        return OPEN_IN_OTHER_MODE_ERR;
    }

    if(thisFile->curr != get_current_process())
    {
        return OPEN_IN_OTHER_PROCESS_ERR;
    }

    return thisFile->state;
    mutex_unlock(mutex);
}

uint64_t closeFile(file * thisFile)
{
    mutex_lock(mutex);
    if (thisFile == NULL)
    {
      return -1;
    }

    if (thisFile->state == CLOSE)
    {
      return 0;
    }

    if (thisFile->curr != get_current_process())
    {
        return -1;
    }
    thisFile->curr = NULL;
    thisFile->state = CLOSE;
    mutex_unlock(mutex);
    return 0;
}
/**********************GETTERS && SETTERS************/

// char * getSonsName(file * f)
// {
//   uint64_t size = f->sonsAmount;
//   char * resp = buddyAllocate (MAX_SONS * (MAX_FILE_NAME + 5));
//   for (int p = 0; p < size ; p++)
//   {
//       // strcat(resp, f->sons[p]->name);
//       // strcat(resp, "  /  ");
//   }
//   return resp;
// }
file * getCurrentDir()
{
    return currentDir;
}

file * getHome()
{
    return home;
}

char * getName(file * f)
{
  return f->name;
}


char isDir(file * f)
{
  return f->isDir;
}


char* pathName(file * f, char * resp)
{
    if (f == home)
    {
      strcpyWithNoIndex(resp,"/home");
      return resp;
    }

    concat(pathName(f->father, resp), '/');
    concat(resp, f->name);
    return resp;
}

// PathNames no pueden terminar con /
file * pathToFile(char * pathName)
{
    char bufferDir[MAX_FILE_NAME];
    file * curr = home;

    uint64_t x = 0;
    uint64_t index = 0;
    uint64_t initialFlag = TRUE;

    while (pathName[x]!= '\0')
    {
        if (pathName[x] =='/')
        {
            if(initialFlag)
            {
                initialFlag = FALSE;
            }
            else
            {
                bufferDir[index] = '\0';
                curr = fileChild(curr, bufferDir);
                if (curr == NULL)
                {
                    return NULL;
                }
                index = 0;
            }
        }
        else
        {
            bufferDir[index] = pathName[x];
            index++;
        }
        x++;
    }
    curr = fileChild(curr, bufferDir);
    return curr;
}

/*
* Return this File if bufferDir is thisFile Name.
* Return childs file if bufferDir is Name is a thisFile'sons name
* else return NULL
*/

file * fileChild(file * thisFile,char * bufferDir)
{
  if (thisFile == NULL)
  {
    return NULL;
  }

  if (!isDir(thisFile))
  {
    return NULL;
  }

  if ( strcmp(thisFile->name,bufferDir) == 0)
  {
    return thisFile;
  }

  for (uint64_t y = 0; y < thisFile->sonsAmount; y ++)
  {
      if (strcmp(thisFile->sons[y]->name, bufferDir) == 0)
      {
        return thisFile->sons[y];
      }
  }
  return NULL;
}

file * getFileFromPath(char * pathName, char * name)
{
  file * father = pathToFile(pathName);
  file * resp = fileChild(father, name);
  return resp;
}
