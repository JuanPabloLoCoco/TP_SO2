#include <stdint.h>
#include <clear.h>

uint64_t _int80(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

/*clears the screen*/
void clear()
{
	_int80(9,0,0,0,0,0);
}
