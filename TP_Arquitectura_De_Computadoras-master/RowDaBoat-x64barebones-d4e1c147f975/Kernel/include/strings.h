#ifndef STRINGS_H
#define STRINGS_H

#define UINT_MAX 4294967295

/* Converts num into a '\0' ended string in str parameter*/
void intToString(char* str, int num);

/* Reverse str */
void reverseCharArray(char* str, int length);

/* Returns the length of num ('-' sign is counted whereas '+' sign isn't) */
int numLength(int num);

/* str1: destination, str2: origin */
int strcpy(char* str1, const char* str2, int length);

/* convert str, '\0' ended string, to uppercase */
void toUpper(char* str);

/* Copia una cadena de caracteres a otra direccion de memoria. Devuelve la cantidad de caracteres copiados */
int strcpyWithNoIndex(char *from, char *to);

int strlen(const char* str) ;

int strcmp(char * s, char * t);
int strncmp(char * s, char * t, unsigned int n);

#endif
