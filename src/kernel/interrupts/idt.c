
#include "idt.h"
#include <stdlib/error.h>

void exception_handler() {
    __asm__ volatile("cli");
    panic("Exception not handled");
}

// idt table
__attribute__((aligned(16))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

// idt table ref struct
static idtr_t idtr;

// refercne asm stub table
extern void** isr_stub_table;

void idt_init() {
    // setup base pointer
    idtr.base = (uint32_t)idt;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    // init first 32 as stubs
    for(uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    // load the idt and enable interrupts
    __asm__ volatile("lidt %0 \n"
                     "sti \n"
                     :
                     : "m"(idtr));
}
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 8; // this value can be whatever offset your kernel
                               // code selector is in your GDT
                               // this comes from gdt.asm (CODE_SEG), it is 8
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}
