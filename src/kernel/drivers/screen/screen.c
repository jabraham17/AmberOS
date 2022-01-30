
#include "screen.h"
#include <lowlevel/io.h>
#include <stdlib/string.h>

int getScreenOffset(int row, int col) { return 2 * (row * SC_MAX_COLS + col); }
int getColFromOffset(int offset) {
    return (offset % (2 * SC_MAX_COLS)) / 2;
}
int getRowFromOffset(int offset) {
    return offset / (2 * SC_MAX_COLS);
}

int getCursorOffset() {
    //read high byte
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_HIGH);
    int offset = io_inb(SC_VGA_DATA_REGISTER) << 8;

    //read low bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_LOW);
    offset |= io_inb(SC_VGA_DATA_REGISTER);

    //convert from char to cell
    offset *= 2;

    return offset;
}
void setCursorOffset(int offset) {
    //convert from cell to char
    int charOffset = offset / 2;

    //write low bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_LOW);
    io_outb(SC_VGA_DATA_REGISTER, (unsigned char) (charOffset & 0xff));

    //write high bytes
    io_outb(SC_VGA_CTRL_REGISTER, SC_VGA_OFFSET_HIGH);
    io_outb(SC_VGA_DATA_REGISTER, (unsigned char) (charOffset >> 8));

    //cursor is moved, we need to set the attribute of this cell
    unsigned char* vidmem = (unsigned char*)SC_VIDEO_ADDRESS;
    vidmem[offset + 1] = SC_WHITE_ON_BLACK;
}

void enableCursor()
{
    unsigned char cursorStart = 0;
    //line size is maxscan line / max rows, we want cursor to take up full line
    
    //read Maximum Scan Line Register, this will be cursor end
    //http://www.osdever.net/FreeVGA/vga/crtcreg.htm#09
	io_outb(SC_VGA_CTRL_REGISTER, 0x09);
    unsigned char cursorEnd = io_inb(SC_VGA_DATA_REGISTER) & 0b00011111;

    // read current state of Cursor Start Register
    //http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
    io_outb(SC_VGA_CTRL_REGISTER, 0x0A);
    cursorStart |= (io_inb(SC_VGA_DATA_REGISTER) & 0b11000000);
    //write it back out
    io_outb(SC_VGA_DATA_REGISTER, cursorStart);

    // read current state of Cursor End Register
    //http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0B
    io_outb(SC_VGA_CTRL_REGISTER, 0x0B);
    cursorEnd |= (io_inb(SC_VGA_DATA_REGISTER) & 0b11100000);
    //write it back out
    io_outb(SC_VGA_DATA_REGISTER, cursorEnd);
}

void SC_init() {
    //set cursor size and put it at offset 0
    enableCursor();
    setCursorOffset(0);
}

void SC_clearScreen() {
    memset(SC_VIDEO_ADDRESS, 0, SC_NCHARS);
    setCursorOffset(0);
}

void SC_printStringAt(char* str, int row, int col) {
    //not valid, print from cursor
    if(row < 0 || col < 0) {
        int offset = getCursorOffset();
        row = getRowFromOffset(offset);
        col = getColFromOffset(offset);
    }
    while(*str != '\0') {
        SC_printCharAt(*str, (int)row, (int)col, 0);
        col++;
        //col wraparound or newline
        if(col >= SC_MAX_COLS || *str == '\n') {
            col = 0;
            row += 1;
        }
        str++;
    }
    int offset = getScreenOffset(row, col);
    setCursorOffset(offset);
}
void SC_printString(char* str) {
    SC_printStringAt(str, -1, -1);
}


void SC_printCharAt(char ch, unsigned int row, unsigned int col, char attribute) {
    
    unsigned char* vidmem = (unsigned char*)SC_VIDEO_ADDRESS;
    // if no attr, use default
    if(attribute == 0) {
        attribute = SC_WHITE_ON_BLACK;
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
        offset = getScreenOffset(getRowFromOffset(offset)+1, 0);
    }
}

