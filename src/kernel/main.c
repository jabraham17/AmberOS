
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

#include <stdlib/string.h>

void main() {
    SC_init();
    SC_clearScreen();

    kb_init();


    SC_printString("Hello, World!\n");
    SC_printString("I am running in the kernel!!!\n");
    SC_printString("Hello, World!\n");
    SC_printString("I am a really long string that will hopefully wrap around "
                   "on the screen because i am so long");
    SC_printString("Separate\n");
    SC_printString("Lets put some tabs\tin here to see how it gets handled\t");
    SC_printString("now we start a new\tsection\n");

    SC_printf("hello my name is %%%s%% and my favorite number is %d, which in hex is 0x%x", "steven", 17, 0x11);

    int big_hex = 0xDEADBEEF;
    SC_printf("\nReally big hex number 0x%hhx, 0x%hx, 0x%x\n", big_hex, big_hex, big_hex);


    return;
}
