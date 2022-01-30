%ifndef _IC_SWITCH32_ASM_
%define _IC_SWITCH32_ASM_

bits 16
switch_32pm:
    cli ; disable interruptd
    lgdt [gdt_def.gdt_descriptor] ; load descriptor

    ; set cr0
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp CODE_SEG:init_32pm  ; far jump to 32 bit code


bits 32
init_32pm:
    mov ax, DATA_SEG ; reset all regment regs to data 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; set stack pointer
    mov esp, STACK_OFFSET_32
    xor ebp, ebp

    call BEGIN_PM

bits 32
BEGIN_PM:
    mov eax, strings.pm_mode
    call print_str_32
    call KERNEL_OFFSET
    ;jmp $

%endif
