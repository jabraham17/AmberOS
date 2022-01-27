%ifndef _IC_START_ASM_
%define _IC_START_ASM_
bits 32
extern main
_start:
    call main
    jmp $

%endif
