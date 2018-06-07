#include <defs.h>
#include <stdio.h>
#include <syscall.h>
#include <pipe.h>
#include <pipeTest.h>

int fd;
pipe_t testPipe = (pipe_t)0;
char * testPipeName = "testPipe";
char * testText = "Prueba";

void runPipeTest()
{
  pipeCreationTest();
  writeReadTest();
}

void pipeCreationTest()
{
  givenAPipe();
  thenVerifyCreation();
}

void givenAPipe()
{
  sys_openPipe((uint64_t ) testPipeName, (uint64_t) &fd);
}

void thenVerifyCreation()
{
  int aux = whereIsPipe(testPipeName);
  if (aux == -1)
  {
    draw_word("CREATION PIPE TEST FAILED");
  }
  else
  {
    draw_word("CREATION PIPE TEST SUCCESS");
    testPipe = getPipe(testPipeName);
  }
}

void writeReadTest()
{
  
}
