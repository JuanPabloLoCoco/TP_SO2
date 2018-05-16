#include <stdint.h>
#include <stdlib.h>
#include <date.h>

uint64_t _int80(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

/*prints the date DD/MM/YY format*/
void date()
{
	int day = (int)_int80(7,0,0,0,0,0);
	int month = (int)_int80(6,0,0,0,0,0);
	int year = (int)_int80(5,0,0,0,0,0);
	printf(" %d/%d/%d\n", day, month, year);
}
