#ifndef _KERNEL_LOWLEVEL_IO_H_
#define _KERNEL_LOWLEVEL_IO_H_

#include <stdlib/types.h>

byte io_inb(word port);
void io_outb(word port, byte data);
word io_inw(word port);
void io_outw(word port, byte data);

#endif