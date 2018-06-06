#include "strings.h"

/* Converts num into a '\0' ended string in str parameter*/
void intToString(char* str, int num)
{
	int length = numLength(num);
	int numLength = length;
	int i;

	if (num < 0)
	{
		str[length - 1] = '-';
		num *= -1;
		numLength--;
	}

	for (i = 0; i < numLength; i++) {
		str[i] = (char)(num % 10 + '0');
		num /= 10;
	}

	reverseCharArray(str, length);
	str[length] = '\0';
	return;
}

void reverseCharArray(char* str, int length)
{
	char aux[length];
	int i;
	strcpy(aux, str, length);

	for (i = 0; i < length; i++)
	{
		str[length - 1 - i] = aux[i];
	}
}

void toUpper(char* str)
{
	while(*str)
	{
		if (*str >= 'a' && *str <= 'z')
		{
			*str += 'A' - 'a';
		}
		str++;
	}
}


int strlen(const char* str)
{
	int length = 0;
	while (*(str++) != '\0')
	{
		length++;
	}
	return length;
}


int strcpy(char* str1, const char* str2, int length)
{
	int i;
	for (i = 0; i < length && str2[i]!='\0'; i++)
	{
		str1[i] = str2[i];
	}
	if(i!=length) str1[i]='\0';
	return i;
}

int strcpyWithNoIndex(char *from, char *to)
{
	int i = 0;
	for (i = 0; from[i] != '\0'; i++)
	{
		to[i] = from[i];
	}
	to[i] = 0;
	return i;
}

void concat(char * str1, char * str2)
{
	uint64_t i = 0;
	uint64_t j = 0;
	while (str1[i] !='\0')
	{
		i++;
	}
	while (str2[j]!= '\0')
	{
		str1[i] = str2[j];
		i++;
		j++;
	}
	str1[i] ='\0'
}


void strCatNum(char* ans,char* str,int num)
{
	int len=strlen(str);
	strcpy(ans,str,-1);
	intToString(ans+len,num);
	return;
}

int numLength(int num)
{
	int length = 1;

	if (num < 0)
	{
		length += 1;
		num *= -1;
	}

	while(num / 10 != 0)
	{
		length += 1;
		num /= 10;
	}
	return length;
}

/* Compara dos cadenas de capacteres */
int strcmp(char * s, char * t) {
	return strncmp(s, t, UINT_MAX);
}

/* Compara dos cadenas de caracteres */
int strncmp(char * s, char * t, unsigned int n) {
	int i;
	for (i = 0; i < n && s[i] != '\0' && t[i] != '\0' && s[i] == t[i]; i++)
		;
	return s[i]-t[i];
}
