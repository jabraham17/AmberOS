#ifndef _KERNEL_INTERRUPTS_PIC_H_
#define _KERNEL_INTERRUPTS_PIC_H_

#include <stdlib/types.h>

// https://wiki.osdev.org/PIC

// pic1 is master
// pic2 is slave
// pic1 (irq0-7) is 0x08 to 0x0F
// pic2 (irq8-15) is 0x70 to 0x77
// we will remap them to interrupts 32-47 (0x20-0x2F)
// set pic1 to 0x20 and pic2 to 0x28
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

// eoi command
#define PIC_EOI 0x20

#define ICW1_ICW4 0x01      /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

#define PIC_READ_IRR 0x0a /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0b /* OCW3 irq service next CMD read */

#define PIC_MAX_INTERRUPTS 16

void pic_disable_irq(unsigned char IRQline);
void pic_enable_irq(unsigned char IRQline);
void pic_send_EOI(unsigned char irq);
__attribute__((always_inline)) inline uint16_t pic_get_irr();
__attribute__((always_inline)) inline uint16_t pic_get_isr();
void pic_init();

#endif
