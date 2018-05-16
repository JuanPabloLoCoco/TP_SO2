#include <stdint.h>
#include <stdlib.h>
#include <date.h>
#include <syscalls.h>

/*prints the date DD/MM/YY format*/
void date()
{
	int day = get_day();
	int month = get_month();
	int year = get_year();
	printf(" %d/%d/%d\n", day, month, year);
}
