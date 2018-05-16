#include <stdint.h>
#include <color.h>

uint64_t _int80(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

/*changes the font color*/
void color(int blue, int green, int red)
{
	_int80(13,blue,green,red,0,0);
}
