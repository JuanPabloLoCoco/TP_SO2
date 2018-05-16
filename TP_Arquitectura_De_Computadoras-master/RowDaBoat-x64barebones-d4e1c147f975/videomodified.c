#include <stdint.h>
#include <videoDriver.h>
#include <modeInfo.h>
#include <font.h>




void set_up_VESA_mode();


   MODE_INFO * mode_info = (MODE_INFO*)0x0000000000005C00;
   Pointer pointer = {0,0};
   Color background_color= {0,0,0};
   void set_up_VESA_mode(){
   	int i,j;
      //draw_char('a');     
      //draw_word("Alejo gilun",0,0);
      //draw_word("Alan gilun a ver que mas puedo decir a ver como queda en la pantalla",0,0);
      //draw_word("Alan gilun a ver que mas puedo decir a ver como queda en la pantalla 10\\/?",500,0);

   }

    void draw_pixel(int x, int y){

      uint8_t * vi =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *y + mode_info->bpp/8*x);
      vi[0] = 0xFF;
      vi[1] = 0xFF;
      vi[2] = 0xFF;
   }

   void draw_horizontalLine(int x0, int x1, int y){
      for(int i=x0; i<x1;i++){
         draw_pixel(i,y);
      }
   }

   void draw_verticalLine(int x, int y0, int y1){
      for(int i=y0; i<y1;i++){
         draw_pixel(x,i);
      }
   }

   void draw_verticalTotalLine(int x){
      draw_verticalLine(x,0,mode_info->height);
   }

   void draw_horizontalTotalLine(int y){
      draw_horizontalLine(0,0,mode_info->width);
   }


    void draw_pixel_with_color(int x, int y,Color color){

      uint8_t * vi =(uint8_t*) (mode_info->framebuffer + mode_info->pitch *y + mode_info->bpp/8*x);
      vi[0] = color.blue;
      vi[1] = color.green;
      vi[2] = color.red;
   }

   void cls(){
      Color c;
      c.red=0;
      c.green=0;
      c.blue=0;
      pointer.x=0;
      pointer.y=0;
      setBackgroundColor(background_color);
   }

   void setBackgroundColor(Color color){
      background_color = color;
      for (int x=0; x<mode_info->height;x++){
         for (int y=0;y<mode_info->width;y++){
            draw_pixel_with_color(x,y, color);
         }
      }
   }

   void draw_char(uint8_t l) {
      char * letter = pixel_map(l);
      int i,j;
      Color white = {0xFF,0xFF,0xFF};
      Color black = {0,0,0};
      if(pointer.x + CHAR_WIDTH - 1 >= mode_info->width) {
         pointer.y+=CHAR_HEIGHT;
         pointer.x = 0;
      }
      for(i = 0; i<CHAR_HEIGHT;i++) {
         for(j = 0; j<CHAR_WIDTH; j++) {
            if(1<<j & letter[i])
               draw_pixel_with_color(CHAR_WIDTH-1-j+pointer.x,i+pointer.y,white);
            else
               draw_pixel_with_color(CHAR_WIDTH-1-j+pointer.x,i+pointer.y,black);   
         }
      }
      pointer.x += CHAR_WIDTH;
   }

   void draw_char_with_color(uint8_t l, int x, int y,Color letter_color, Color background_color) {
      char * letter = pixel_map(l);
      int i,j;
      if(pointer.x + CHAR_WIDTH - 1 >= mode_info->width)
         pointer.y+=CHAR_HEIGHT;
      for(i = 0; i<CHAR_HEIGHT;i++) {
         for(j = 0; j<CHAR_WIDTH; j++) {
            if(1<<j & letter[i])
               draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,letter_color);
            else
               draw_pixel_with_color(CHAR_WIDTH-1-j+x,i+y,background_color);   
         }
      }  
      pointer.x += CHAR_WIDTH;
   }

   void draw_word(char * word,int x, int y) {

      int i = 0;
      while(word[i]) {
         draw_char(word[i]);
         i++;
      }
   }

   void erase_char(){
      if(pointer.x == 0){
         if(pointer.y != 0) {
            pointer.y = pointer.y - CHAR_HEIGHT;
            pointer.x = mode_info->width - CHAR_WIDTH;
         }
         else
            return;
      }
      else       
         pointer.x = pointer.x - CHAR_WIDTH;
      draw_char(' ');
      pointer.x = pointer.x - CHAR_WIDTH;
   }

   void newLine(){
      pointer.y = pointer.y + CHAR_HEIGHT;
      pointer.x = 0;
   }

void draw_horizontalTotalLine(int y);
void draw_verticalTotalLine(int x);
void draw_verticalLine(int x, int y0, int y1);
void draw_horizontalLine(int x0, int x1, int y);
void setBackgroundColor(Color color);
void cls();