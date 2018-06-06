#ifndef FILETDD_H
#define FILETDD_H

void runFStest();

void createDirTest();
void givenAFile();
void thenVerifyCreation();

void testBlockFile();
void whenAddBlock(File * thisFile);
void thenVerifyBlockAdd();

void testWriteFile();
char * givenAtextToWrite();
uint64_t whenWriteInFile(file * thisFile, void * bytes, uint64_t count);
void thenVerifyTextWasWtitten(uint64_t bytesToWrite, uint64_t bytesNoWritten);

void testRead();
void whenFileIsRead(file * file, char * text);
void thenVerifyTextWasRead(char * textRead);

void deleteDirTest();
uint64_t whenFileIsDelete();
void thenVerifyFileIsDelete(uint64_t resp);

#endif
