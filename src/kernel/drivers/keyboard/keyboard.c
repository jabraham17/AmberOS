#include "keyboard.h"
#include <lowlevel/io.h>
#include <interrupts/isr.h>
#include <drivers/screen/screen.h>


void kb_isr() {
    unsigned char b = io_inb(0x60);
    SC_printf("Got a char: 0x%hhx %hhd\n", b);
}

void kb_init() {
    isr_install(1, kb_isr);
}
