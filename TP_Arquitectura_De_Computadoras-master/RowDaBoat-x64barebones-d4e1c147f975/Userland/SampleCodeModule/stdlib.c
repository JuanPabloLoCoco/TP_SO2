//stdlib.c
#include <stdint.h>
#include <stdlib.h>
#include <plotter.h>
#include <stdarg.h>
extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

/*returns 1 if the given char is a letter, returns 0 if not*/
int isAlpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/*returns 1 if the given char is a digit, returns 0 if not*/
int isNum(char c)
{
	return (c >= '0' && c <= '9');
}

/*returns 1 if the given char is a space, returns 0 if not*/
int isSpace(char c)
{
	return (c == ' ');
}

/*turns the given integer into a string and stores it in
** the given array
*/
void intToStr(int num, char * ret)
{
	int dig = 0;
	char aux;
	if(num != 0)
	{
		if(num < 0)
		{
			num = -num;
			ret[0] = '-';
			dig++;
		}
		while(num != 0){
			ret[dig] = (num % 10) + '0';
			num = num/10;
			dig++;
		}
		for(int x = 0; x < dig/2; x++)
		{
			aux = ret[x];
			ret[x] = ret[dig-x-1];
			ret[dig-x-1] = aux;
		}
		ret[dig] = 0;
	}
	else
	{
		ret[0] = '0';
	}
}

/*reverse the given string*/
void strrev(char * str)
{
	int length = strlen(str);
	char aux[length];
	int i;
	strcpy(str, aux);
	for (i = 0; i < length; i++)
	{
		str[length - 1 - i] = aux[i];
	}
}

/*returns the length of the given string*/
int strlen(const char * str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		i++;
	}
	return i;
}

/*copys the first string into the second string*/
int strcpy(char *from, char *to)
{
	int i = 0;
	for (i = 0; from[i] != '\0'; i++)
	{
		to[i] = from[i];
	}
	to[i] = 0;
	return i;
}

/*returns 0 if the given strings are diferent and 1 if they are equals*/
int strcmp(char * array1, char * array2)
{
	int i = 0;
	for (i = 0; array2[i] != '\0'; i++)
	{
		if(array1[i] != array2[i])
		{
			return 0;
		}
	}
	return array1[i] == array2[i];
}

/*prints the given string replacing %d with the integer
** parameter received and %s with the string parameter
** and %c with the char parameter received
*/
void printf(const char * str,...)
{
	char num[12];
	va_list valist;
	va_start (valist,str);
	int length = strlen(str);
	int state = 0;
	for(int x = 0; x < length; x++)
	{
		if(state == 0)
		{
			if(str[x] != '%')
			{
				putchar(str[x]);
			}
			else
			{
				state = 1;
			}
		}
		else
		{
			switch(str[x])
			{
				case 'd':
					intToStr(va_arg (valist, int),num);
					int index = 0;
					while(num[index] != 0)
						putchar(num[index++]);
					state = 0;
					break;
				case 'c':
					putchar((char)(va_arg(valist,int)));
					state = 0;
					break;
				case 's':
					printf(va_arg(valist, char*));
					state = 0;
					break;
				default:
					putchar('%');
					putchar(str[x]);
					state = 0;
					break;
			}
		}
	}
	va_end(valist);
}

/* print the given char on screen*/
void putchar(const char c)
{
	_int80((uint64_t)1,(uint64_t)1,(uint64_t)&c,(uint64_t)1,(uint64_t)0,(uint64_t)0);
}

/*reads a char from the buffer and returns it*/
int getchar()
{
	char c;
	_int80((uint64_t)0,(uint64_t)0,(uint64_t)&c,(uint64_t)1,(uint64_t)0,(uint64_t)0);
	return c;
}

/*reads from the input the given format and when finds a %d or %c or %s
** reads and stores its content in the int, char, or char* respectively that
** were given
*/
int scanf(const char* format,...)
{
	va_list args;
	va_start( args, format );
	int quantity = 0;
	int i = 0, j = 0;
	char* read = readLine();
	int* number;
  char* string;

	while(format[i])
	{
		if(format[i] != '%')
		{
      if((format[i]) != (read[j]))
			{
          return quantity;
      }
      else
			{
           i++;
           j++;
      }
		}
		else
		{
			i++;
			switch(format[i])
			{
				case '%':
          if(read[j] != '%')
          	return quantity;
          j++;
          break;
				case 'd':
					number = va_arg(args,int*);
					*number = 0;
					while(isNum(read[j]))
					{
						*number = (*number) *10 + read[j] - 48;
						j++;
					}
				  quantity++;
					break;
				case 'c':
          string = va_arg(args, char*);
          *string = read[j];
          j++;
          quantity++;
          break;
				case 's':
          string = va_arg(args,char*);
          char c;
          while( c = read[j] )
					{
              *string = c;
              string++;
              j++;
          }
          quantity++;
					break;
			}
			i++;
		}
	}
	return quantity;
}

/*reads text from the input until enter and returns what has read*/
char* readLine()
{
    int bufferIndex = 0;
    char buff[BUFFER_SIZE];
    int c ;
    while ((c = getchar()) != '\n')
		{
        if(c == '\b')
				{
            if (bufferIndex != 0)
						{
                bufferIndex--;
            }
        }
        else
				{
            if (bufferIndex <= BUFFER_SIZE)
						{
                buff[bufferIndex++] = c;
            }
            putchar(c);
        }
    }
    putchar(c);
    buff[bufferIndex] = '\0';
    return (char *)buff;
}

uint64_t malloc(uint64_t amount)
{
	return _int80(14, amount, 0, 0, 0, 0);
}

void free(uint64_t address)
{
	_int80(15, address, 0, 0, 0, 0);
}

void ps(uint64_t address, uint64_t amount)
{
	_int80(17, address, amount, 0, 0, 0);
}
