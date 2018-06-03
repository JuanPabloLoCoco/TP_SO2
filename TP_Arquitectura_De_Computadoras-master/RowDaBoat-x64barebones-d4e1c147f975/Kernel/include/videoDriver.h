#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#define RED_MASK 0x00FF0000
#define GREEN_MASK 0x00FF00
#define BLUE_MASK 0x0000FF


typedef struct Color {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} Color;

typedef struct pointer {
	int x;
	int y;
}Pointer;


/*sets the pointer to the given values*/
void setPointer(int x, int y);

/*draw a pixel with the current font color in the given position*/
void draw_pixel(int x, int y);

/*draws a pixel in the given position and with the given color*/
void draw_pixel_with_color(int x, int y,Color color);

/*prints the given char on the screen at the pointers position*/
void draw_char(uint8_t l);

/*draw the given char on the screen at the given position*/
void draw_char_position(uint8_t l, int x, int y);

/*draw the given char on the screen at the given position and with
** the given color
*/
void draw_char_with_color(uint8_t l, int x, int y,Color letter_color, Color background_color);

/*prints the given string starting at the current pointers position*/
void draw_word(char * word);

/*deletes the last char printed*/
void erase_char();

/*prints a new line at the current pointers position*/
void newLine();
/*scroll up the screen copying to each line of the screen
** its following and leaving the last one empty
*/
void scrollUp(void);

/*deletes the line of height y*/
void eraseLine(int y);

/*copys into the line of height to the line of height from*/
void copyLine(int to, int from);

/*clears the screen*/
void cls();

/*paints all the screen with the given color*/
void setBackgroundColor(Color color);

/*returns the screen height*/
int getScreenHeigth(void);

/*returns the screen width*/
int getScreenWidth(void);

/*prints the given number in its hexadecimal representation*/
void printHexaNumber(uint64_t number);

void setFontColor(uint8_t blue, uint8_t green, uint8_t red);

/*returns the char corresponding to the hexadecimal representation
** of the given number, the given number must be between 0 and 15
** if not it will return 0;
*/

void printNum(int num);

#endif
