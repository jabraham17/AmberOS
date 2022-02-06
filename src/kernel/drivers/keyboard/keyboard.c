#include "keyboard.h"
#include <drivers/screen/screen.h>
#include <interrupts/isr.h>
#include <lowlevel/io.h>

void kb_isr() {

    unsigned char b = io_inb(0x60);
    unsigned char b2 = io_inb(0x60);
    SC_printf("Got a char: 0x%hhx %hhx\n", b, b2);
}

static inline void kb_wait(void) {
    int i;
    for(i = 0; i < 0x10000; i++)
        if((io_inb(KB_STATUS_PORT) & 0x02) == 0)
            break;
}

static inline void send_cmd(unsigned char c) {
    kb_wait();
    io_outb(KB_COMMAND_PORT, c);
}

void kb_init() {

    // while(!(io_inb(KB_STATUS_PORT) & 0x02))

    send_cmd(0xAD); //disable first port
    send_cmd(0xA7); //disable second port

    io_inb(KB_DATA_PORT);//flush buffer

    kb_wait();
    io_outb(KB_DATA_PORT, 0x00);
    send_cmd(0xAA);
    kb_wait();
    char config = io_inb(KB_DATA_PORT);
    SC_printf("            76543210\n");
    SC_printf("response: 0b%hhb 0x%hhx\n", config, config);

    send_cmd(0xAE); //enable first port
    send_cmd(0xA8); //enable second port
    isr_install(1, kb_isr);
}
