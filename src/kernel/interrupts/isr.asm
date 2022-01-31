
%ifndef _IC_KERNEL_INTERRUPTS_IDT_ASM_
%define _IC_KERNEL_INTERRUPTS_IDT_ASM_
bits 32

extern exception_handler

%macro isr_exception 1
isr_exception_%+%1:
    pushad
    cld ; C code following the sysV ABI requires DF to be clear on function entry
    cli
    push esp
    push %1
    call exception_handler
    add esp, 8 ; pop nowhere
    sti
    popad
    iret
%endmacro

%assign i 0 
%rep 32 
isr_exception i
%assign i i+1 
%endrep

global exception_stub_table
exception_stub_table:
%assign i 0 
%rep 32 
    dd isr_exception_%+i
%assign i i+1 
%endrep

%endif
