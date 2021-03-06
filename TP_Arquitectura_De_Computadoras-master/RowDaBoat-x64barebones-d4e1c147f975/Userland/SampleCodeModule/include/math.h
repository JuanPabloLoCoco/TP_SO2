#ifndef MATH_H
#define MATH_H
#define M_2_PI 6.28318530718f
#define M_PI_2 1.57079632679f
#define M_PI 3.14159265359f

void cos(float x, float * res);

void sin(float x, float * res);

void srand(unsigned int seed);

int randM(void); // RAND_MAX assumed to be 32767

int randBound(int min, int max);

int rand_int();

int rand_int_range(int low, int top);


#endif
