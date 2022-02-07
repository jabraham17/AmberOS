
#include "screen.h"
#include <lowlevel/io.h>
#include <stdlib/string.h>

unsigned char SC_globalAttribute;
unsigned char SC_cursorAttribute;

int getScreenOffset(int row, int col) { return 2 * (row * SC_MAX_COLS + col); }
int getColFromOffset(int offset) { return (offset % (2 * SC_MAX_COLS)) / 2; }
int getRowFromOffset(int offset) { return offset / (2 * SC_MAX_COLS); }

int getCursorOffset() {
    // read high byte
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_HIGH);
    int offset = io_inb(SC_VGA_DATA_REGISTER) << 8;

    // read low bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_LOW);
    offset |= io_inb(SC_VGA_DATA_REGISTER);

    // convert from char to cell
    offset *= 2;

    return offset;
}
void setCursorOffset(int offset) {

    //get old cursor offset
    int oldOffset = getCursorOffset();

    // convert from cell to char
    int charOffset = offset / 2;

    // write low bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_LOW);
    io_outb(SC_VGA_DATA_REGISTER, (unsigned char)(charOffset & 0xff));

    // write high bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_HIGH);
    io_outb(SC_VGA_DATA_REGISTER, (unsigned char)(charOffset >> 8));

    // cursor is moved, we need to set the attribute of this cell
    unsigned char* vidmem = (unsigned char*)SC_VIDEO_ADDRESS;
    vidmem[offset + 1] = SC_cursorAttribute;
    vidmem[oldOffset + 1] = SC_globalAttribute;
    //change old cursor offset to global color
    
}

void enableCursor() {
    unsigned char cursorStart = 0;
    // line size is maxscan line / max rows, we want cursor to take up full line

    // read Maximum Scan Line Register, this will be cursor end
    // http://www.osdever.net/FreeVGA/vga/crtcreg.htm#09
    io_outb(SC_VGA_CTRL_REGISTER, 0x09);
    unsigned char cursorEnd = io_inb(SC_VGA_DATA_REGISTER) & 0b00011111;

    // read current state of Cursor Start Register
    // http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
    io_outb(SC_VGA_CTRL_REGISTER, 0x0A);
    cursorStart |= (io_inb(SC_VGA_DATA_REGISTER) & 0b11000000);
    // write it back out
    io_outb(SC_VGA_DATA_REGISTER, cursorStart);

    // read current state of Cursor End Register
    // http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0B
    io_outb(SC_VGA_CTRL_REGISTER, 0x0B);
    cursorEnd |= (io_inb(SC_VGA_DATA_REGISTER) & 0b11100000);
    // write it back out
    io_outb(SC_VGA_DATA_REGISTER, cursorEnd);
}

void SC_init() {
    SC_setGlobalAttribute(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    SC_setCursorAttribute(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    // set cursor size and put it at offset 0
    enableCursor();
    setCursorOffset(0);
}

void SC_setGlobalAttribute(vga_color_t fg, vga_color_t bg) {
    SC_globalAttribute = SC_VGA_COLOR_ATTR(fg, bg);
}
void SC_setCursorAttribute(vga_color_t fg, vga_color_t bg) {
    SC_cursorAttribute = SC_VGA_COLOR_ATTR(fg, bg);
}

void SC_clearScreen() {
    uint16_t clearByte = (SC_globalAttribute << 8) | 0;
    uint16_t* videomem = (uint16_t*)SC_VIDEO_ADDRESS;
    for(size_t i = 0; i < SC_NCELLS; i++) {
        videomem[i] = clearByte;
    }
    setCursorOffset(0);
}

void scroller() {
    unsigned char* vidmem = (unsigned char*)SC_VIDEO_ADDRESS;
    // copy 1:MAX_ROWS-1 lines to 0:MAX_ROWS-2 lines, set cursor to start of
    // last line
    int srcOffset = getScreenOffset(1, 0);
    int dstOffset = getScreenOffset(0, 0);
    int nBytes = SC_ROWSIZE * (SC_MAX_ROWS - 1);
    // because ptrs overlap, must use memmove
    memmove(vidmem + dstOffset, vidmem + srcOffset, nBytes);

    // clear last line
    int lastLineOffset = getScreenOffset(SC_MAX_ROWS - 1, 0);
    memset(vidmem + lastLineOffset, 0, SC_ROWSIZE);

    // set cursor
    setCursorOffset(lastLineOffset);
}

void SC_printStringAt(char* str, int row, int col) {
    // not valid, print from cursor
    if(row < 0 || col < 0) {
        int offset = getCursorOffset();
        row = getRowFromOffset(offset);
        col = getColFromOffset(offset);
    }
    while(*str != '\0') {
        if(*str == '\t') {
            // tab size is 4
            SC_printCharAt(' ', (int)row, (int)col, 0);
            SC_printCharAt(' ', (int)row, (int)col + 1, 0);
            SC_printCharAt(' ', (int)row, (int)col + 2, 0);
            SC_printCharAt(' ', (int)row, (int)col + 3, 0);
            col += 4;
        } else if(*str == '\n') {
            col = 0;
            row += 1;
        } else {
            SC_printCharAt(*str, (int)row, (int)col, 0);
            col++;
        }
        // wraparound
        if(col >= SC_MAX_COLS) {
            col = col - SC_MAX_COLS;
            row += 1;
        }
        if(row >= SC_MAX_ROWS) {
            row = SC_MAX_ROWS - 1; // max out row
            scroller();
        }
        str++;
    }
    int offset = getScreenOffset(row, col);
    setCursorOffset(offset);
}
void SC_printString(char* str) { SC_printStringAt(str, -1, -1); }

char printf_buffer[1024];
void SC_printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(printf_buffer, fmt, args);
    va_end();
    SC_printString(printf_buffer);
}
void SC_printCharAt(
    char ch, unsigned int row, unsigned int col, char attribute) {

    unsigned char* vidmem = (unsigned char*)SC_VIDEO_ADDRESS;
    // if no attr, use default
    if(attribute == 0) {
        attribute = SC_globalAttribute;
    }

    int offset = getScreenOffset(row, col);

    // not a newline, dump out char
    if(ch != '\n') {
        vidmem[offset] = ch;
        vidmem[offset + 1] = attribute;
        offset += 2;
    }
    // newline, set offset to be next line
    else {
        offset = getScreenOffset(getRowFromOffset(offset) + 1, 0);
    }

    if(offset >= SC_NCHARS) {
        scroller();
    }
}

#if defined(DEBUG) && DEBUG == 1
void SC_debugColorMatrix() {
    SC_setGlobalAttribute(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    SC_setCursorAttribute(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    SC_clearScreen();
    int combos = 1;
    for(int bg = 0; bg < 16; bg++) {
        for(int fg = 0; fg < 16; fg++) {
            SC_setGlobalAttribute(fg, bg);
            SC_printf("AaZz");
            if(combos % 16 == 0) {
                SC_printf("\n");
            }
            combos++;
        }
    }
}
#endif
