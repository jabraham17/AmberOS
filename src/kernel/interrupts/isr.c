#include "isr.h"
#include "idt.h"
#include "pic.h"

extern void* pic_isr_handlers[16];
extern void* pic_isr_stub_table[16];
extern void isr_empty_func();


void isr_install(unsigned char isr, void* handler) {
    __asm__ volatile("cli");
    pic_isr_handlers[isr] = handler;
    pic_enable_irq(isr);
    __asm__ volatile("sti");
}
void isr_remove(unsigned char isr) {
    __asm__ volatile("cli");
    pic_isr_handlers[isr] = isr_empty_func;
    pic_disable_irq(isr);
    __asm__ volatile("sti");
}

void isr_init() {
    __asm__ volatile("cli");
    //install all the stubs
    for(int i = 0; i < 16; i++) {
        idt_set_descriptor(32+i, pic_isr_stub_table[i], IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT);
        pic_disable_irq(i);
    } 
    __asm__ volatile("sti");
}
