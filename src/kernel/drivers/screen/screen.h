#ifndef _KERNEL_DRIVER_SCREEN_H_
#define _KERNEL_DRIVER_SCREEN_H_

#define SC_VIDEO_ADDRESS ((void*)0xB8000)
#define SC_MAX_ROWS 25
#define SC_MAX_COLS 80
#define SC_NCELLS (SC_MAX_ROWS * SC_MAX_COLS)
#define SC_NCHARS (SC_NCELLS * 2)
#define SC_ROWSIZE (SC_MAX_COLS * 2)

// Screen device I/O ports
#define SC_VGA_CTRL_REGISTER 0x03D4
#define SC_VGA_DATA_REGISTER 0x03D5
#define SC_VGA_OFFSET_HIGH 0x0E
#define SC_VGA_OFFSET_LOW 0x0F

typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color_t;

#define SC_VGA_COLOR_ATTR(FG, BG) ((FG) | (BG) << 4)

// cursor control: http://www.osdever.net/FreeVGA/vga/crtcreg.htm

void SC_init();
void SC_setGlobalAttribute(vga_color_t fg, vga_color_t bg);
void SC_setCursorAttribute(vga_color_t fg, vga_color_t bg);

void SC_clearScreen();
void SC_printStringAt(char* str, int row, int col);
void SC_printString(char* str);
void SC_printf(char* fmt, ...);
void SC_printCharAt(
    char ch, unsigned int row, unsigned int col, char attribute);

#if defined(DEBUG) && DEBUG == 1
void SC_debugColorMatrix();
#endif

#endif
