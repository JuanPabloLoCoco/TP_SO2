#include <shell.h>
#include <stdlib.h>
#include <stdint.h>

void testDualAllocation()
{
	printf("\nTesteo doble creacion:\n");
	void * firstAddress = malloc(1000);
	void * secondAddress = malloc(1000);
	if(firstAddress == secondAddress)
	{
		printf("Mismos lugares. Mala alocacion\n");
	}
	else
	{
		printf("Distintos lugares. Correcta alocacion\n");
	}
}

void testFree()
{
	printf("\nTesteo free:\n");
	void * firstAddress = malloc(1000);
	free(firstAddress);
	void * secondAddress = malloc(1000);
	if(firstAddress == secondAddress)
	{
		printf("Mismos lugares. Buen free\n");
	}
	else
	{
		printf("Distintos lugares. Mal free\n");
	}
}
