#include <shell.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t _int80(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void testDualAllocation()
{
	printf("Testeo doble creacion:\n");
	uint64_t firstAddress = malloc(1000);
	uint64_t secondAddress = malloc(1000);
	if((int)firstAddress == (int)secondAddress)
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
	printf("Testeo free:\n");
	uint64_t firstAddress = malloc(1000);
	free(firstAddress);
	uint64_t secondAddress = malloc(1000);
	if((int)firstAddress == (int)secondAddress)
	{
		printf("Mismos lugares. Buen free\n");
	}
	else
	{
		printf("Distintos lugares. Mal free\n");
	}
}

void testPS()
{
	char* address = malloc(3000);
	ps(address, 3000);
	printf("%s\n",address );
	free(address);
	int l;
	printf("Presione q para volver\n");
	while((l = getchar()) != 'q');
}
