#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <syscalls.h>

/*prints the time on screen in HH:MM:SS format*/
void time()
{
	int hours = get_hours();
	int minutes = get_minutes();
	int seconds = get_seconds();

	printf(" %d:%d:%d\n", hours, minutes, seconds);
}
