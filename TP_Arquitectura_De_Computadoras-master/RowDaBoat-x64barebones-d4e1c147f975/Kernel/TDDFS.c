#include <TDDFS.h>

#include <buddyAllocator.h>

file * testFile = NULL;
fileBlock * freeBlocks2 = NULL;
char * testFileName = "testFile";
char * testText = "Prueba";

void runFStest()
{
    createDirTest();
    testWriteFile();
    testRead();
    deleteDirTest();
}

void createDirTest()
{
    givenAFile();
    thenVerifyCreation();
}

void givenAFile()
{
    testFile = createNotDir(testFileName, getHome());
}

void thenVerifyCreation()
{
    if (testFile == NULL)
    {
        draw_word("CREATION TEST FAIL");
        return;
    }
    draw_word("CREATION TEST PASSED");
}


void testWriteFile()
{
    char * text = givenAtextToWrite();
    // +1. strlen no suma al '\0';
    uint64_t count = strlen(text) + 1;
    uint64_t resp = whenWriteInFile(testFile,(void *) text, count);
    thenVerifyTextWasWtitten(count, resp);
}

char * givenAtextToWrite()
{
  return testText;
}

uint64_t whenWriteInFile(file * thisFile, char* bytes, uint64_t count)
{
    return writeOnFile(thisFile, bytes, count);
}

void thenVerifyTextWasWtitten(uint64_t bytesToWrite, uint64_t bytesNoWritten)
{
    if (bytesNoWritten == 0)
    {
        draw_word("Write was sucesfull. Test Pass! \n");
        return;
    }
    if (bytesToWrite == bytesNoWritten)
    {
        draw_word("Nothing was written \n");
        return;
    }
    draw_word("Fail on Writting!");
    return;
}

void testRead()
{
    char * readText = (char *) buddyAllocate(64);
    whenFileIsRead(testFile,readText);
    thenVerifyTextWasRead(readText);
    buddyFree(readText);
}

void whenFileIsRead(file * file, char * text)
{
    uint64_t index = 0;
    char c;
    while ( (c = (char *)readFile(file, index)) != EOF )
    {
        text[index] = c;
        index++;
    }
    text[index] ='\0';
}

void thenVerifyTextWasRead(char * textRead)
{
    if (strcmp(testRead, testText) == 0)
    {
        draw_word(" Test de Lectura fue exitoso \n");
    }
    else
    {
        draw_word("Test de Lectura Fallo. \n");
    }
}

void deleteDirTest()
{
    uint64_t resp = whenFileIsDelete();
    thenVerifyFileIsDelete(resp);
}

uint64_t whenFileIsDelete()
{
    uint64_t resp = deleteFile(testFile);
}

void thenVerifyFileIsDelete(uint64_t resp)
{
    if (resp == 0)
    {
        draw_word("Deletion Of TestFile Was sucesfull \n");
        return;
    }
    draw_word("Deletion Faile");
}
