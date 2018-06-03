#include <plotter.h>
#include <stdint.h>
#include <stdlib.h>
#include <syscalls.h>

  /*draws a pixel in the given position with the given color*/
	void draw_pixel_with_color(int x, int y,Color color)
	{
				sys_paint(x , y , color.blue , color.green , color.red );
	}

  /*draws a pixel in the given position*/
	void draw_pixel(int x, int y)
  {
    	sys_paint(x,y, 0, 0, 0);
  }

  /*draws and horizontal line from the x0 collumn to the x1 collumn in the y row*/
  void draw_horizontalLine(int x0, int x1, int y)
  {
  	for(int i = x0; i < x1; i++)
    {
      	draw_pixel(i, y);
    }
 	}

  /*draw a vertical line from the y0 row to the y1 row in the x collumn*/
 	void draw_verticalLine(int x, int y0, int y1)
  {
    	for(int i = y0; i < y1; i++)
      {
      	draw_pixel(x, i);
    	}
 	}

  /*draw a vertical line of length height at the x collumn*/
 	void draw_verticalTotalLine(int x)
  {
    	draw_verticalLine(x,0,(int)getScreenHeight());
 	}


  /*draw a line line of length width at the y row*/
 	void draw_horizontalTotalLine(int y)
  {
    	draw_horizontalLine(0,(int)getScreenWidht(),y);
 	}

  /*plots the function corresponding to a*x^2 + b*x +c equation*/
  void drawFunction(int a, int b, int c)
  {
      int height = (int)getScreenHeight();
      int width = (int)getScreenWidht();
      drawAxis();
      double x = 0;
      double y = 0;
      double scale = 0.01;
      for (int p = -500; p < 500; p++)
      {
         x = p*scale;
         y = a*x*x+b*x+c;
         y *= 100;
         draw_pixel(width/2 + p, height/2-y);
      }
   }

   /*draw the axis in wich will plot the function*/
   void drawAxis()
   {
      int height = (int)getScreenHeight();
      int width = (int)getScreenWidht();
      int x0 = width/2;
      draw_verticalTotalLine(x0);
      int y0 = height/2;
      draw_horizontalTotalLine(y0);
			sys_drawCharPosition("1", width/2 + 100, height/2 +CHAR_HEIGHT );
			sys_drawCharPosition("2", width/2 + 200, height/2 +CHAR_HEIGHT );
			sys_drawCharPosition("3", width/2 + 300, height/2 +CHAR_HEIGHT );
      sys_drawCharPosition("3",width/2+300,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("4",width/2+400,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("5",width/2+500,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("1",width/2-100,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("2",width/2-200,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("3",width/2-300,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("4",width/2-400,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("5",width/2-500,height/2 +CHAR_HEIGHT);
      sys_drawCharPosition("1",width/2-CHAR_WIDTH,height/2 -100);
      sys_drawCharPosition("2",width/2-CHAR_WIDTH,height/2 -200);
      sys_drawCharPosition("3",width/2-CHAR_WIDTH,height/2 -300);
      sys_drawCharPosition("4",width/2-CHAR_WIDTH,height/2 -400);
      sys_drawCharPosition("5",width/2-CHAR_WIDTH,height/2 -500);
      sys_drawCharPosition("1",width/2-CHAR_WIDTH,height/2 +100);
      sys_drawCharPosition("2",width/2-CHAR_WIDTH,height/2 +200);
      sys_drawCharPosition("3",width/2-CHAR_WIDTH,height/2 +300);
      sys_drawCharPosition("4",width/2-CHAR_WIDTH,height/2 +400);
      sys_drawCharPosition("5",width/2-CHAR_WIDTH,height/2 +500);
   }

  /*clears the screen*/
  void cls()
  {
     	clear();
 	}

  /*returns the screen width*/
  uint64_t getScreenWidht()
  {
    return get_screen_info(0);
  }

  /*returns the screen height*/
  uint64_t getScreenHeight()
  {
    return get_screen_info(1);
  }
