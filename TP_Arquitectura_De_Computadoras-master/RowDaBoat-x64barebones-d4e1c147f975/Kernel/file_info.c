#include <file_info.h>
#include <strings.h>
#include <file.h>

void get_file_info(file_info * fi, file * thisFile)
{
    strcpyWithNoIndex(fi->name, thisFile->name);
    fi->isDir = thisFile->isDir;
    fi->sonsAmount = thisFile->sonsAmount;
    for (uint64_t i = 0; i < thisFile->sonsAmount ; i++)
    {
      strcpyWithNoIndex(fi->sons[i], thisFile->sons[i]);
    }
}
