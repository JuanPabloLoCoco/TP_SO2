#include <stdlib.h>
#include <time.h>
#include <stdint.h>

uint64_t _int80(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

/*prints the time on screen in HH:MM:SS format*/
void time()
{
	int hours = (int)_int80(4,0,0,0,0,0);
	int minutes = (int)_int80(3,0,0,0,0,0);
	int seconds = (int)_int80(2,0,0,0,0,0);

	printf(" %d:%d:%d\n", hours, minutes, seconds);
}
