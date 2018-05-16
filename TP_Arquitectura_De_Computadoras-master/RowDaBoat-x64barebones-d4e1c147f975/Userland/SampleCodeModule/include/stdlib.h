#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

#define BUFFER_SIZE 256

/*returns 1 if the given char is a letter, returns 0 if not*/
int isAlpha(char c);

/*returns 1 if the given char is a digit, returns 0 if not*/
int isNum(char c);

/*returns 1 if the given char is a space, returns 0 if not*/
int isSpace(char c);

/*turns the given integer into a string and stores it in
** the given array
*/
void intToStr(int num,char * ret);

/*reverse the given string*/
void strrev(char * str);

/*returns the length of the given string*/
int strlen(const char * str);

/*copys the first string into the second string*/
int strcpy(char *from, char *to);

/*returns 0 if the given strings are diferent and 1 if they are equals*/
int strcmp(char * array1, char * array2);

/*prints the given string replacing %d with the integer
** parameter received and %s with the string parameter
** and %c with the char parameter received
*/
void printf(const char * str,...);

/* print the given char on screen*/
void putchar(const char c);

/*reads a char from the buffer and returns it*/
int getchar(void);

/*reads from the input the given format and when finds a %d or %c or %s
** reads and stores its content in the int, char, or char* respectively that
** were given
*/
int scanf(const char* format,...);

/*reads text from the input until enter and returns what has read*/
char* readLine(void);

uint64_t malloc(uint64_t amount);
void free(uint64_t address);
void ps(uint64_t address, uint64_t amount);

#endif
