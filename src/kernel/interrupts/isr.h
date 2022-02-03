#ifndef _KERNEL_INTERRUPTS_ISR_H_
#define _KERNEL_INTERRUPTS_ISR_H_


void isr_install(unsigned char isr, void* handler);
void isr_remove(unsigned char isr);
void isr_init();

#endif
