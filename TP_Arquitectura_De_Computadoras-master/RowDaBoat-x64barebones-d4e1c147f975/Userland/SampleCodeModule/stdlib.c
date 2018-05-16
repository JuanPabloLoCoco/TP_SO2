//stdlib.c
#include <stdint.h>
#include <stdlib.h>
#include <plotter.h>
#include <stdarg.h>
#include <syscalls.h>

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
	syscall_write(1, &c, 1);
}

/*reads a char from the buffer and returns it*/
int getchar()
{
	char c;
	syscall_read(0,&c,1);
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

/* Copia en str los valores ascii de los digitos de value en la base indicada.
** Devuelve la cantidad de digitos copiados. */
int itoa(int value, char *str, int base) {
	char *p = str;
	char *p1, *p2;
	int len = 0;

	if(value < 0 && base == 10)
	{
		value = -value;
		len++;
		*p++ = '-';
		str++;
	}
	//Calculate characters for each digit
	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		len++;
	}
	while (value /= base);

	// Terminate string in str.
	*p = '\0';

	//Reverse string in str.
	p1 = str;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return len;
}

/* A partir de un string retorna su valor entero */
int atoi(const char *str)
{
	while (isspace(*str))
		str++;

	int num = 0;
	int sign = 1;
	if (*str == '-')
	{
		str++;
		sign = -1;
	}
	else if (*str == '+')
		str++;

	while (*str != '\0' && isdigit(*str))
	{
		num = num * 10 + (*str) - '0';
		str++;
	}

	return num * sign;
}

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void * malloc (int size)
{
	return allocMemory(size);
}

void free(void * ptr)
{
	freeMemory(ptr);
}
