
#include <drivers/screen/screen.h>
#include <drivers/uart/uart.h>
#include <lowlevel/io.h>

#include <drivers/uart/uart.h>

void main() {


    SC_init();
    SC_clearScreen();
    SC_printString("Hello, World!\n");
    SC_printString("Hello, World!\n");
    SC_printString("I am a really long string that will hopefully wrap around "
                   "on the screen because i am so long");
    SC_printString("Separate\n");

    // while(!(io_inb(0x64)& 0b00000001));
    // char b = io_inb(0x60);
    // SC_printString("Got CHar\n");

    //__asm__ volatile ("1:jmp 1b");

    return;
}
