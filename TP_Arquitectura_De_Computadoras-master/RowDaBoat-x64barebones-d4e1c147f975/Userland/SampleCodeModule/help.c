#include <stdlib.h>

/*prints the menu with the avaliable commands and their description*/
void help()
{
	printf("   time: Prints time in HH:MM:SS format\n\n");
	printf("   date: Prints date in DD/MM/YY format\n\n");
	printf("   clear: Clears screen\n\n");
	printf("   echo [STRING]: displays the given string, if the string length is greater than 50 it will be shortened to 50\n\n");
	printf("   linear  a b: Plots the function corresponding to the linear equation y = ax + b\n\n");
	printf("   quadratic  a b c: Plots the function corresponding to the quadratic equation y = ax^2 + bx + c\n\n");
	printf("   color  b g r: change the font color to the color given b(blue) g(green) r(red) each of them between 0 and 256\n\n");
	printf("   test  id: tests the exception corresponding to the given id 0 for zeroDivision, 1 for invalidOpCode ,2 for overflow\n");
	printf("             ,3 for breakpoint.\n\n");
	printf("   dual allocation test: Test dual allocation with same size\n\n");
	printf("   free test: allocation-free-allocation and checl if the address is the same\n\n");
	printf("   ps test: create and ps an address\n\n");
}
