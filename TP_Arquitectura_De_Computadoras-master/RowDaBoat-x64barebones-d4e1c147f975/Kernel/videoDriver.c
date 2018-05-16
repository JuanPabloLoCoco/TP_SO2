#include <stdint.h>
#include <videoDriver.h>
#include <modeInfo.h>
#include <font.h>
#include <string.h>

MODE_INFO * mode_info = (MODE_INFO*)0x0000000000005C00;
Pointer pointer = {0,0};
Color background_color= {0,0,0};
Color fontColor = {0xFF,0xFF,0xFF};
int lastEnter = -1;

static char toHexa(uint64_t n);

/*sets the pointer to the given values*/
void setPointer(int xvalue, int yvalue)
{
   if(xvalue >= 0 && xvalue< mode_info->width && yvalue >= 0 && yvalue< mode_info->height)
   {
         pointer.x = xvalue;
         pointer.y = yvalue;
   }
}

/*draw a pixel with the current font color in the given position*/
void draw_pixel(int x, int y)
{
   uint8_t * vi =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *y + mode_info->bpp/8*x);
   vi[0] = fontColor.blue;
   vi[1] = fontColor.green;
   vi[2] = fontColor.red;
}

/*draws a pixel in the given position and with the given color*/
void draw_pixel_with_color(int x, int y,Color color)
{
   uint8_t * vi =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *y + mode_info->bpp/8*x);
   vi[0] = color.blue;
   vi[1] = color.green;
   vi[2] = color.red;
}

/*prints the given char on the screen at the pointers position*/
void draw_char(uint8_t l)
{
  if (l == '\n')
  {
    newLine();
  }
  char * letter = pixel_map(l);
  int i,j;
  if(pointer.x + CHAR_WIDTH - 1 >= mode_info->width)
  {
    lastEnter = mode_info->width-1;
    if(pointer.y +CHAR_HEIGHT >= mode_info->height)
    {
      scrollUp();
      pointer.y = mode_info->height - CHAR_HEIGHT;
    }
    else
    {
      pointer.y+=CHAR_HEIGHT;
    }
      pointer.x = 0;
   }
   for(i = 0; i<CHAR_HEIGHT;i++)
   {
      for(j = 0; j<CHAR_WIDTH; j++)
      {
         if(1<<j & letter[i])
            draw_pixel_with_color(CHAR_WIDTH-1-j+pointer.x,i+pointer.y,fontColor);
         else
            draw_pixel_with_color(CHAR_WIDTH-1-j+pointer.x,i+pointer.y,background_color);
      }
   }
   pointer.x += CHAR_WIDTH;
}

   /*draw the given char on the screen at the given position*/
void draw_char_position(uint8_t l, int x, int y)
{
    char * letter = pixel_map(l);
    int i,j;
    Color white = {0xFF,0xFF,0xFF};
    Color black = {0,0,0};
    if(x + CHAR_WIDTH - 1 >= mode_info->width)
    {
       y+=CHAR_HEIGHT;
       x = 0;
    }
    for(i = 0; i<CHAR_HEIGHT;i++)
    {
       for(j = 0; j<CHAR_WIDTH; j++)
       {
          if(1<<j & letter[i])
             draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,white);
          else
             draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,black);
       }
    }
    x += CHAR_WIDTH;
}

 /*draw the given char on the screen at the given position and with
 ** the given color
 */
 void draw_char_with_color(uint8_t l, int x, int y,Color letter_color, Color background_color)
 {
    char * letter = pixel_map(l);
    int i,j;
    if(pointer.x + CHAR_WIDTH - 1 >= mode_info->width)
       pointer.y+=CHAR_HEIGHT;
    for(i = 0; i<CHAR_HEIGHT;i++)
    {
       for(j = 0; j<CHAR_WIDTH; j++)
       {
          if(1<<j & letter[i])
             draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,letter_color);
          else
             draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,background_color);
       }
    }
    pointer.x += CHAR_WIDTH;
 }

 /*prints the given string starting at the current pointers position*/
 void draw_word(char * word)
 {
    int i = 0;
    for  (; word[i] != '\0'; i++)
    {
       draw_char(word[i]);
    }
 }

 /*deletes the last char printed*/
 void erase_char()
 {
    if(pointer.x == 0)
    {
       if(pointer.y != 0)
       {
          pointer.y = pointer.y - CHAR_HEIGHT;
          pointer.x = lastEnter - CHAR_WIDTH;
          lastEnter = mode_info->width;
       }
       else
       {
          return;
       }

    }
    else
    {
       pointer.x = pointer.x - CHAR_WIDTH;
    }
    draw_char(' ');
    pointer.x = pointer.x - CHAR_WIDTH;
 }

   /*prints a new line at the current pointers position*/
void newLine()
{
  lastEnter = pointer.x;
  if(pointer.y = mode_info->height - CHAR_HEIGHT)
  {
    scrollUp();
    pointer.y = mode_info->height - CHAR_HEIGHT;
  }
  else
    pointer.y = pointer.y + CHAR_HEIGHT;
    pointer.x = 0;
}

 /*scroll up the screen copying to each line of the screen
 ** its following and leaving the last one empty
 */
 void scrollUp()
 {
    int i;
    static int quantity = 0;
    int from = mode_info->height - (3+quantity)*CHAR_HEIGHT;
    if(from < 0)
    {
       from = 0;
    }
    for(i = from; i<pointer.y; i+=CHAR_HEIGHT)
    {
       copyLine(i,i+CHAR_HEIGHT);
    }
    eraseLine(pointer.y);
    quantity++;
 }

 /*deletes the line of height y*/
void eraseLine(int y)
{
    int i,limit = mode_info->bpp * mode_info->width/CHAR_WIDTH;
    pointer.y = y;
    for(i = 0; i<= limit;i++)
    {
       draw_char_with_color(' ',i,y,background_color,background_color);
    }
}

   /*copys into the line of height to the line of height from*/
 void copyLine(int to, int from)
 {
    uint8_t * v1 =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *to);
    uint8_t * v2 =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *from);
    int x,y;
    for(y=0;y<4*CHAR_HEIGHT;y++)
    { //probamos hasta que con 4 dio
      for(x = 0 ; x < mode_info->width; x++)
      {
        v1[0] = v2[0];
        v1[1] = v1[1];
        v2[2] = v2[2];

        v1+=1;
        v2+=1;
       }
    }
 }

   /*clears the screen*/
 void cls(){
    Color c;
    c.red=0;
    c.green=0;
    c.blue=0;
    pointer.x=0;
    pointer.y=0;
    setBackgroundColor(background_color);
 }

   /*paints all the screen with the given color*/
 void setBackgroundColor(Color color){
    background_color = color;
    for (int y=0; y<mode_info->height;y++)
    {
       for (int x=0;x<mode_info->width;x++)
       {
          draw_pixel_with_color(x,y, color);
       }
    }
 }

   /*returns the screen height*/
 int getScreenHeigth()
 {
    return mode_info->height;
 }

   /*returns the screen width*/
 int getScreenWidth()
 {
    return mode_info->width;
 }

   /*sets the font color to the given color*/
 void setFontColor(uint8_t blue, uint8_t green, uint8_t red)
 {
    fontColor.blue = blue;
    fontColor.green = green;
    fontColor.red = red;
 }

   /*prints the given number in its hexadecimal representation*/
 void printHexaNumber(uint64_t number)
 {
    char n[16] ={0};
    int index = 0,i;
    do
    {
       n[15 - index] = toHexa(number%16);
       number = number/16;
       index++;
    }while(number);

    for(i = 0; i<16; i++)
    {
       if(n[i])
          draw_char(n[i]);
    }
 }

   /*returns the char corresponding to the hexadecimal representation
   ** of the given number, the given number must be between 0 and 15
   ** if not it will return 0;
   */
 static char toHexa(uint64_t n)
 {
    if(n>=0 && n<=9)
       return (char)(n+48);
    switch(n)
    {
       case 10:
          return 'A';
       case 11:
          return 'B';
       case 12:
          return 'C';
       case 13:
          return 'D';
       case 14:
          return 'E';
       case 15:
          return 'F';
    }
    return 0;
 }

 void printNum(int num)
 {
   char str[10];
   intToString(str, num);
   draw_word(str);
 }
