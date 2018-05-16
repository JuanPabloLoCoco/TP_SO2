#ifndef _SHELL_H_
#define _SHELL_H_

#define CHAR_HEIGHT 16

#define TIME 1
#define DATE 2
#define CLEAR 3
#define HELP 4
#define ECHO 5
#define LINEAR 6
#define QUADRATIC 7
#define COLOR 8
#define TEST 9
#define DUAL_ALLOCATION_TEST 10
#define FREE_TEST 11
#define PS_TEST 12
#define COMMAND_MAX_LENGTH 10
#define COMMANDS_QUANTITY 20
#define COMMANDS_MAX_ARGS 50
#define MAX_LINEAR_INTS 2
#define MAX_QUADRATIC_INTS 3
#define MAX_COLOR_INTS 3
#define ID_INTS 1


void shell(void);

/*set up the pointer to the las line*/
void setUpShell(void) ;

/*prints the prompt*/
void printPrompt(void);

/*read the input and returns a value corresponding to the
** command read.
*/
int getCommands(void);

/*reads from the input until enter and returns 0 if could be read
** 1 if there were more backspaces than typed chars
*/
int readArgs(char * args);

/*stores the numbers of the argument read in an integer vector
** and returns how many ints were given
*/
int getInts(int totalArgs);

/*turns the string starting in args at the position pos
** into its corresponding integer value
*/
int getNumber(char* args, int* pos);

/*set the screen to plot a function and then plots it
** when q is pressed clears the screen and returns to
** shell screen mode.
*/
void functionScreen(int a, int b, int c);

/*returns 1 if all of the integer numbers read in the
** argument are between from and to, returns 0 if not
*/
int valid(int from,int to,int size);

void prodcons();

void producer();

void consumer();

#endif
