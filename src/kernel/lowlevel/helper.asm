%ifndef _IC_KERNEL_LOWLEVEL_HELPER_ASM_
%define _IC_KERNEL_LOWLEVEL_HELPER_ASM_

global halt
global debugBreak

halt:
    jmp $
    ret

debugBreak:
    int3
    ret

%endif
