
%ifndef _IC_KERNEL_INTERRUPTS_IDT_ASM_
%define _IC_KERNEL_INTERRUPTS_IDT_ASM_
bits 32

extern exception_handler

%macro isr_exception 1
isr_exception_%1:
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

%macro make_isr_exception_entry 1
    dd isr_exception_%1
%endmacro 

global exception_stub_table
exception_stub_table:
%assign i 0 
%rep 32 
    make_isr_exception_entry i
%assign i i+1 
%endrep



extern pic_send_EOI

%macro isr_pic 1
isr_pic_%1:
    pushad
    cld
    cli

    ; have to load ptr to func from table before calling it
    mov eax, [(pic_isr_handlers + (%1*4))]
    call eax

    push %1
    call pic_send_EOI
    add esp, 4 ; pop nowhere

    sti
    popad
    iret
%endmacro

%assign i 0 
%rep 16 
isr_pic i
%assign i i+1 
%endrep

global isr_empty_func
isr_empty_func:
    ret

global pic_isr_handlers
pic_isr_handlers:
%assign i 0 
%rep 16
    dd isr_empty_func
%assign i i+1 
%endrep


%macro make_isr_pic_entry 1
    dd isr_pic_%1
%endmacro 

global pic_isr_stub_table
pic_isr_stub_table:
%assign i 0 
%rep 16
    make_isr_pic_entry i
%assign i i+1 
%endrep

%endif
