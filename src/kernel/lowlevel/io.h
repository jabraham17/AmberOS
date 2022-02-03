#ifndef _KERNEL_LOWLEVEL_IO_H_
#define _KERNEL_LOWLEVEL_IO_H_

#include <stdlib/types.h>

byte io_inb(word port);
void io_outb(word port, byte data);
word io_inw(word port);
void io_outw(word port, byte data);
__attribute__((always_inline)) static inline void io_wait() {
    io_outb(0x80, 0);
}

#endif
