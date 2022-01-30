
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

        panic(0);


    return;
}
