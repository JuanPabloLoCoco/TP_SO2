#ifndef MODE_INFO_H
#define MODE_INFO_H


//http://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
//http://wiki.osdev.org/Drawing_In_Protected_Mode#Drawing_Text
typedef struct MODE_INFO {
   uint16_t attributes;      __attribute__ ((packed));// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
   uint8_t window_a;         __attribute__ ((packed));// deprecated
   uint8_t window_b;         __attribute__ ((packed));// deprecated
   uint16_t granularity;     __attribute__ ((packed));// deprecated; used while calculating bank numbers
   uint16_t window_size;     __attribute__ ((packed));
   uint16_t segment_a;       __attribute__ ((packed));
   uint16_t segment_b;       __attribute__ ((packed));
   uint32_t win_func_ptr;    __attribute__ ((packed));// deprecated; used to switch banks from protected mode without returning to real mode
   uint16_t pitch;           __attribute__ ((packed));// number of bytes per horizontal line
   uint16_t width;           __attribute__ ((packed));// width in pixels
   uint16_t height;          __attribute__ ((packed));// height in pixels
   uint8_t w_char;           __attribute__ ((packed));// unused...
   uint8_t y_char;           __attribute__ ((packed));// ...
   uint8_t planes;           __attribute__ ((packed));
   uint8_t bpp;              __attribute__ ((packed));// bits per pixel in this mode
   uint8_t banks;            __attribute__ ((packed));// deprecated; total number of banks in this mode
   uint8_t memory_model;     __attribute__ ((packed));
   uint8_t bank_size;        __attribute__ ((packed));// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
   uint8_t image_pages;      __attribute__ ((packed));
   uint8_t reserved0;        __attribute__ ((packed));

   uint8_t red_mask;         __attribute__ ((packed));
   uint8_t red_position;     __attribute__ ((packed));
   uint8_t green_mask;       __attribute__ ((packed));
   uint8_t green_position;   __attribute__ ((packed));
   uint8_t blue_mask;        __attribute__ ((packed));
   uint8_t blue_position;    __attribute__ ((packed));
   uint8_t reserved_mask;    __attribute__ ((packed));
   uint8_t reserved_position;         __attribute__ ((packed));
   uint8_t direct_color_attributes;   __attribute__ ((packed));

   uint32_t framebuffer;           __attribute__ ((packed));// physical address of the linear frame buffer; write here to draw to the screen
   uint32_t off_screen_mem_off;    __attribute__ ((packed));
   uint16_t off_screen_mem_size;   __attribute__ ((packed));// size of memory in the framebuffer but not being displayed on the screen
   uint8_t reserved1[206];         __attribute__ ((packed));
} MODE_INFO;

#endif
