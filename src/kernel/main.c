
#include "drivers/screen/screen.h"
#include "lowlevel/helper.h"

void setmem() {
        char* mem = 0xB8000;
    mem[0] = 'h';
    mem[2] = 'e';
    mem[4] = 'l';
}

void main() {
    setmem();
    SC_clearScreen();
        setmem();
    SC_printCharAt('X', 0, 0, 0);
    // SC_printCharAt('X', 0, 0, 0);
    // SC_clearScreen();
    // SC_printString("Hello, World");
    return;
}
