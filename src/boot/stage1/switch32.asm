%ifndef _IC_STAGE1_SWITCH32_ASM_
%define _IC_STAGE1_SWITCH32_ASM_

bits 16
switch_32pm:
    cli ; disable interruptd
    lgdt [gdt_def.gdt_descriptor] ; load descriptor

    ; set cr0
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp CODE_SEG:.flush  ; far jump to 32 bit code

; here we are in 32 bit land
bits 32
.flush:
    mov ax, DATA_SEG ; reset all regment regs to data 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp KERNEL_OFFSET ; we never return here

%endif
