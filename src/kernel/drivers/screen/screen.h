#ifndef _KERNEL_DRIVER_SCREEN_H_
#define _KERNEL_DRIVER_SCREEN_H_

#define SC_VIDEO_ADDRESS ((void*)0xB8000)
#define SC_MAX_ROWS 25
#define SC_MAX_COLS 80
#define SC_NCELLS (SC_MAX_ROWS*SC_MAX_COLS)
#define SC_NCHARS (SC_NCELLS*2)
#define SC_ROWSIZE (SC_MAX_COLS*2)
// Attribute byte for our default colour scheme.
#define SC_WHITE_ON_BLACK 0x0F
// Screen device I/O ports
#define SC_VGA_CTRL_REGISTER 0x03D4
#define SC_VGA_DATA_REGISTER 0x03D5
#define SC_VGA_OFFSET_HIGH 0x0E
#define SC_VGA_OFFSET_LOW 0x0F

//cursor control: http://www.osdever.net/FreeVGA/vga/crtcreg.htm

void SC_init();

void SC_clearScreen();
void SC_printStringAt(char* str, int row, int col);
void SC_printString(char* str);
void SC_printCharAt(char ch, unsigned int row, unsigned int col, char attribute);

#endif
