#ifndef FILETDD_H
#define FILETDD_H

#include <file.h>
#include <videoDriver.h>
#include <strings.h>
#include <defs.h>
#include <stdint.h>

void runFStest();

void createDirTest();
void givenAFile();
void thenVerifyCreation();


void testWriteFile();
char * givenAtextToWrite();
uint64_t whenWriteInFile(file * thisFile, char* bytes, uint64_t count);
void thenVerifyTextWasWtitten(uint64_t bytesToWrite, uint64_t bytesNoWritten);

void testRead();
void whenFileIsRead(file * file, char * text);
void thenVerifyTextWasRead(char * textRead);

void deleteDirTest();
uint64_t whenFileIsDelete();
void thenVerifyFileIsDelete(uint64_t resp);

#endif
