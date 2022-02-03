
#include "idt.h"
#include <drivers/uart/uart.h>
#include <stdlib/error.h>
#include <stdlib/string.h>

// list all 32 exceptions
#define EXCEPTIONS(V)                                                          \
    V("Divide By Zero", noerr)                                                 \
    V("Debug", noerr)                                                          \
    V("NMI", noerr)                                                            \
    V("Breakpoint", noerr)                                                     \
    V("Overflow", noerr)                                                       \
    V("OOB", noerr)                                                            \
    V("Invalid Opcode", noerr)                                                 \
    V("Device Not Available", noerr)                                           \
    V("Double Fault", err)                                                     \
    V("Coprocessor Segment Overrun", noerr)                                    \
    V("Invalid TSS", err)                                                      \
    V("Segment Not Present", err)                                             \
    V("Stack-Segment Fault", err)                                              \
    V("GPF", err)                                                              \
    V("PF", err)                                                               \
    V("RESERVED", noerr)                                                       \
    V("x87 FP Exception", noerr)                                               \
    V("Alignment Check", err)                                                  \
    V("Machine Check", noerr)                                                  \
    V("SIMD FP Exception", noerr)                                              \
    V("Virtualization Exception", noerr)                                       \
    V("Control Protection Exception", err)                                     \
    V("RESERVED", noerr)                                                       \
    V("RESERVED", noerr)                                                       \
    V("RESERVED", noerr)                                                       \
    V("RESERVED", noerr)                                                       \
    V("RESERVED", noerr)                                                       \
    V("RESERVED", noerr)                                                       \
    V("Hypervisor Injection Exception", noerr)                                 \
    V("VMM Communication Exception", noerr)                                    \
    V("Security Exception", err)                                               \
    V("RESERVED", noerr)

#define MAKE_EXCEPTION_NAME(name, type) name,

char* exception_stub_table_names[IDT_CPU_EXCEPTION_COUNT] = {
    EXCEPTIONS(MAKE_EXCEPTION_NAME)};


typedef struct {
    uint32_t EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
    uint32_t eip, cs, efl, useresp, ss;
} exception_isr_state_t;


char exception_buf[128];

void exception_handler(uint32_t exceptionID, exception_isr_state_t* isr) {

    sprintf(exception_buf, "Exception occurred: \'%s\'\nID: %d\nEIP: 0x%x\n", exception_stub_table_names[exceptionID], exceptionID, isr->eip);
    uart_puts(exception_buf);
}

extern void* exception_stub_table[IDT_CPU_EXCEPTION_COUNT];

#undef MAKE_EXCEPTION_NAME
#undef EXCEPTIONS

// idt table
__attribute__((aligned(16))) idt_entry_t idt[IDT_MAX_DESCRIPTORS];

// idt table ref struct
idtr_t idtr;

void idt_init() {
    // setup base pointer
    idtr.base = (uint32_t)idt;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    // init first 32 as stubs for exceptions
    for(uint8_t vector = 0; vector < IDT_CPU_EXCEPTION_COUNT; vector++) {
        idt_set_descriptor(
            vector, exception_stub_table[vector],
            IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT |
                IDT_DESCRIPTOR_RING3);
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

