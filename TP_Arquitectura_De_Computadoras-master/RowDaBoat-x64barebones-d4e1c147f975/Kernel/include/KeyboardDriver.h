#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#define BUFFERSIZE 128
#define ESCAPE 1

char getKey(void);
void keyboard_handler();
int isShiftKey(char k);
int processKeyScanCode(char k);
int chooseKeyboard(int k);
int getChar(void);
int isAlpha(char k);

#endif
