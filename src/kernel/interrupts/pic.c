
#include "pic.h"
#include <lowlevel/io.h>

// offset1 is for pic1 (master), offset2 is for pic2 (slave)
void pic_remap(int offset1, int offset2) {

    byte a1 = io_inb(PIC1_DATA); // save masks
    byte a2 = io_inb(PIC2_DATA);

    // starts the initialization sequence (in cascade mode)
    io_outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    io_outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2: Master PIC vector offset
    io_outb(PIC1_DATA, offset1);
    io_wait();

    // ICW2: Slave PIC vector offset
    io_outb(PIC2_DATA, offset2);
    io_wait();

    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_outb(PIC1_DATA, 4);
    io_wait();

    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_outb(PIC2_DATA, 2);
    io_wait();

    io_outb(PIC1_DATA, ICW4_8086);
    io_wait();
    io_outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // restore saved masks.
    io_outb(PIC1_DATA, a1);
    io_outb(PIC2_DATA, a2);
}

void pic_disable_irq(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = io_inb(port) | (1 << IRQline);
    io_outb(port, value);
}

void pic_enable_irq(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = io_inb(port) & ~(1 << IRQline);
    io_outb(port, value);
}

// if irq on slave, need to send to both
// if irq on master, just master
void pic_send_EOI(unsigned char irq) {
    if(irq >= 8) {
        io_outb(PIC2_COMMAND, PIC_EOI);
    }
    io_outb(PIC1_COMMAND, PIC_EOI);
}

uint16_t pic_get_irq_reg(int ocw3) {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    io_outb(PIC1_COMMAND, ocw3);
    io_outb(PIC2_COMMAND, ocw3);
    return (io_inb(PIC2_COMMAND) << 8) | io_inb(PIC1_COMMAND);
}
/* Returns the combined value of the cascaded PICs irq request register */
__attribute__((always_inline)) inline uint16_t pic_get_irr() {
    return pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
__attribute__((always_inline)) inline uint16_t pic_get_isr() {
    return pic_get_irq_reg(PIC_READ_ISR);
}

void pic_init() {
    __asm__ volatile("cli");
    pic_remap(0x20, 0x28);
    __asm__ volatile("sti");
}
