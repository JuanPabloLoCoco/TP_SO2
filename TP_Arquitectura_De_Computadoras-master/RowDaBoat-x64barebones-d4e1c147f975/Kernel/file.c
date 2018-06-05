#include "include/lib.h"
#include "include/buddyAllocator.h"
#include <file.h>
#include <defs.h>
#include "include/strings.h"
#include <scheduler.h>

static file * currentDir = NULL;
static file * home = NULL;
static fileBlock * freeBlocks = NULL;

static int mutex = 0;


char respPathName [128];

static uint64_t writeOnFile_wr(file * thisfile, void * bytes, uint64_t count);


void initFileSystem()
{
  //Inicializar MEMORIA
  initializeFileBlocks();
  mutex = mutex_open(FILESYSTEMMUTEX);
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
  //char resp [128];
  if (f == home)
  {
    strcpyWithNoIndex(respPathName,"/home");
    return respPathName;
  }
  strcpyWithNoIndex(respPathName, pathName(f->father));
  // strcat(resp, f->name);
  return respPathName;
}


file * createDir(char * name , file * father)
{
    file * new_file = (file *) buddyAllocate( sizeof(file));
    strcpy( new_file->name, name, MAX_FILE_NAME);
    new_file->father = father;
    new_file->sonsAmount = 0;
    new_file->isDir = 0;
    new_file->blocks = NULL;
    new_file->index = 0;
    new_file->state = CLOSE;
    new_file->curr = NULL;

    return new_file;
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
  block->index = 0;
  block->next = freeBlocks;
  freeBlocks = block;
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

/****************writeOn File *************/

uint64_t writeOnFile(file * thisfile, void * bytes, uint64_t count)
{
    uint64_t resp = writeOnFile_wr(thisfile, bytes, count);
    thisfile->index = count - resp;
    realeseBlock(file);
}

static uint64_t writeOnFile_wr(file * thisfile, void * bytes, uint64_t count)
{
    if (thisfile->blocks == NULL)
    {
        thisfile->block = getBlock();
        if (thisfile->block = NULL)
        {
          return count;
        }
    }

    fileBlock * current = thisfile->blocks;

    uint64_t index = 0;

    while (count > 0)
    {
        current->adress[index%BLOCKSIZE] = bytes[index];
        count--;
        index++;

        if (index%BLOCKSIZE == 0 and count > 0)
        {
            if (current->next == NULL)
            {
                current->next = getBlock();
                if ( current->next == NULL)
                {
                    current->adress[(index -1) %BLOCKSIZE] = EOF;
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
            current->adress[(index-1)%BLOCKSIZE] = EOF;
            return count -1;
          }
        }
        current = current->next;
      }

      current->adress[index%BLOCKSIZE] = EOF;
      return count;
}

/******************READ ON FILE ***************/

void * readFile(File * file, uint64_t index)
{
    fileBlock * currentBlock = file->blocks;
    uint64_t currentIndex = file->index;

    if (currentBlock == NULL || currentIndex == 0)
    {
        return EOF;
    }

    while (currentIndex > BLOCKSIZE -1 )
    {
        currentIndex = currentIndex-BLOCKSIZE;
        currentBlock = currentBlock->next;
        if (currentBlock == NULL)
        {
            return EOF;
        }
    }
    return currentBlock->adress[index%BLOCKSIZE];
}

/******************OPEN FILE ************/
uint64_t openFile(file * thisFile, uint64_t state)
{
    mutex_lock(mutex);
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
    mutex_unlock(mutex);
}

void closeFile()
{

}
