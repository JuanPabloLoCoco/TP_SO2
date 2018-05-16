#include <stdint.h>
#include "include/math.h"

int isPowerOfTwo (uint64_t x)
{
    return ((x != 0) && !(x & (x - 1)));
}

uint64_t pow2(uint64_t n)
{
    return (uint64_t)1 << n;
}

uint64_t roundUpPower2(uint64_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}

int lowerBound2Pow(int n)
{
    int i=0;
    while(n)
    {
        n=n>>1;
        i++;
    }
    return 1<<i;
}
