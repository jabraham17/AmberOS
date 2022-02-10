
#include "timers.h"

#include <interrupts/isr.h>
#include <lowlevel/io.h>

#if defined(DEBUG) && DEBUG == 1
#include <drivers/uart/uart.h>
#endif 

uint32_t pit_tick = 0;

void pit_isr() { 
    pit_tick++;
    #if defined(DEBUG) && DEBUG == 1
    uart_printf("Tick: %d\n", pit_tick);
    #endif 
 }

uint32_t pit_getTick() { return pit_tick; }

void pit_init(uint16_t frequency) {

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint16_t divisor = 1193180 / frequency;

    // Send the command byte.
    io_outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    io_outb(0x40, l);
    io_outb(0x40, h);

    isr_install(0, pit_isr);
}
