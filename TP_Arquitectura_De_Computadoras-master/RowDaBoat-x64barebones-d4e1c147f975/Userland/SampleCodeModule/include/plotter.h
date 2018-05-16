#ifndef _PLOTTER_H_
#define _PLOTTER_H_

#include <stdint.h>

#define RED_MASK 0x00FF0000
#define GREEN_MASK 0x00FF00
#define BLUE_MASK 0x0000FF
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

typedef struct Color {
	uint64_t blue;
	uint64_t green;
	uint64_t red;
} Color;

typedef struct pointer {
	int x;
	int y;
}Pointer;


/*draws a pixel in the given position with the given color*/
void draw_pixel_with_color(int x, int y,Color color);

/*draws a pixel in the given position*/
void draw_pixel(int x, int y);

/*draw a horizontal line from the x0 collumn to the x1 collumn in the y row*/
void draw_horizontalLine(int x0, int x1, int y);

/*draw a vertical line from the y0 row to the y1 row in the x collumn*/
void draw_verticalLine(int x, int y0, int y1);

/*draw a vertical line of length height at the x collumn*/
void draw_verticalTotalLine(int x);

/*draw a line line of length width at the y row*/
void draw_horizontalTotalLine(int y);

/*plots the function corresponding to a*x^2 + b*x +c equation*/
void drawFunction(int a, int b, int c);

/*draw the axis in wich will plot the function*/
void drawAxis();

/*clears the screen*/
void cls();

/*returns the screen width*/
uint64_t getScreenWidht();

/*returns the screen height*/
uint64_t getScreenHeight();

#endif