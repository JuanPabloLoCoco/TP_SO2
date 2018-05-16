#include <test.h>
#include <stdlib.h>

/*generates the exception corresponding to the given index*/
void test(int id)
{
	switch(id)
	{
		case ZERODIVISION:
			zeroDivisionTest();
			break;
		case INVALIDOPCODE:
			invalidOpCodeTest();
			break;
		case OVERFLOW:
			overflowTest();
			break;
		// case GENERALPROTECTIONFAULT: generalProtectionFaultTest();
		// 		break;
		case BREAKPOINT:
			breakpointTest();
			break;
	}

}

void zeroDivisionTest()
{
	int i = 10/0;
}

// void generalProtectionFaultTest() {
// 	int * p = -1;
// 	*p = 10;
// }
