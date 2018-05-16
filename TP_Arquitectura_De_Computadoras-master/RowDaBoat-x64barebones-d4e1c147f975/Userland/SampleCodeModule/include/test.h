#ifndef _TEST_H_
#define _TEST_H_

#define ZERODIVISION 0
#define INVALIDOPCODE 1
#define OVERFLOW 2
//#define GENERALPROTECTIONFAULT 3
#define BREAKPOINT 3

/*generates the exception corresponding to the given index*/
void test(int id);

void zeroDivisionTest(void);

void invalidOpCodeTest(void);

void overflowTest(void);

void breakpointTest(void);

#endif